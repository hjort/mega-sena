-- http://g1.globo.com/loterias/megasena.html

-- inserir concursos
INSERT INTO concurso (
  data_concurso, valor_acumulado, numeros_sorteados,
  ganhadores_sena, premiacao_sena, ganhadores_quina, premiacao_quina, ganhadores_quadra, premiacao_quadra)
VALUES
  ('2015-11-14', 104328919.97, '{9,10,36,50,53,55}', null, null, 176, 55056.18, 17821, 776.76),   -- CONCURSO 1761
  ('2015-11-18', 129954301.54, '{26,32,42,45,55,59}', null, null, 219, 63520.19, 18595, 1068.71), -- CONCURSO 1762
  ('2015-11-21', 162026211.93, '{9,12,15,21,31,36}', null, null, 689, 25269.18, 43184, 575.95),   -- CONCURSO 1763
  ('2015-11-25', null, '{6,7,29,39,41,55}', 1, 205329753.89, 401, 58622.54, 33850, 992.09);       -- CONCURSO 1764

INSERT INTO concurso (data_concurso) VALUES ('2015-11-29');

-- calcular valores totais pagos
UPDATE concurso
SET valor_pago =
  coalesce(ganhadores_sena, 0) * coalesce(premiacao_sena, 0) +
  coalesce(ganhadores_quina, 0) * coalesce(premiacao_quina, 0) +
  coalesce(ganhadores_quadra, 0) * coalesce(premiacao_quadra, 0)
WHERE valor_pago IS NULL;

-- inserir unidades lotéricas
INSERT INTO loterica (municipio, uf)
VALUES
  ('Brasília', 'DF'),
  ('Manaus', 'AM'),
  ('Curitiba', 'PR'),
  ('Campinas', 'SP');

TRUNCATE aposta, calculo;

-- inserir apostas
INSERT INTO aposta (id_concurso, id_loterica, data_hora, numeros)
SELECT c.id AS id_concurso,
  ceil(random() * (SELECT max(id) FROM loterica)) AS id_loterica,
  c.data_concurso - (ceil(random() * 3 * 24 * 60) || ' minutes')::interval AS data_hora, -- 3 dias
  sortear_numeros(1, 60, 6, 15) AS numeros
FROM generate_series(1, 10000) a, concurso c
WHERE c.id = 5;

-- efetuar o sorteio
UPDATE concurso
SET numeros_sorteados = sortear_numeros(1, 60, 6, 6)
WHERE id = 5;

-- calcular hash do sorteio
UPDATE concurso
SET hash_sorteio = calcular_hash(numeros_sorteados::varchar::int2[])
WHERE id = 5;

TRUNCATE calculo;

-- calcular hash das apostas
INSERT INTO calculo (id, hash)
SELECT id, calcular_hash(numeros::varchar::int2[]) AS hash
FROM aposta
WHERE id_concurso = 5;

-- calcular acertos de cada aposta
UPDATE calculo b
SET acertos = calcular_acertos(hash, hash_sorteio)
FROM aposta a JOIN concurso c ON (c.id = a.id_concurso)
WHERE b.id = a.id AND c.id = 5;

-- resultados e estatísticas do concurso
select count(1), min(acertos), max(acertos), avg(acertos), stddev(acertos) from calculo;
select acertos, count(1) AS qtd from calculo group by acertos order by acertos desc;
select a.id, id_loterica, data_hora, numeros, acertos from calculo c join aposta a on (a.id = c.id) where acertos = (select max(acertos) from calculo);

