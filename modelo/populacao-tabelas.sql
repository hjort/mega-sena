-- http://g1.globo.com/loterias/megasena.html

INSERT INTO sorteio (
  data_concurso, valor_acumulado, numeros_sorteados,
  ganhadores_sena, premiacao_sena, ganhadores_quina, premiacao_quina, ganhadores_quadra, premiacao_quadra)
VALUES
  ('2015-11-14', 104328919.97, '{9,10,36,50,53,55}', null, null, 176, 55056.18, 17821, 776.76),   -- CONCURSO 1761
  ('2015-11-18', 129954301.54, '{26,32,42,45,55,59}', null, null, 219, 63520.19, 18595, 1068.71), -- CONCURSO 1762
  ('2015-11-21', 162026211.93, '{9,12,15,21,31,36}', null, null, 689, 25269.18, 43184, 575.95),   -- CONCURSO 1763
  ('2015-11-25', null, '{6,7,29,39,41,55}', 1, 205329753.89, 401, 58622.54, 33850, 992.09);       -- CONCURSO 1764

UPDATE sorteio
SET valor_pago =
  coalesce(ganhadores_sena, 0) * coalesce(premiacao_sena, 0) +
  coalesce(ganhadores_quina, 0) * coalesce(premiacao_quina, 0) +
  coalesce(ganhadores_quadra, 0) * coalesce(premiacao_quadra, 0)
WHERE valor_pago IS NULL;

------------

CREATE TABLE loterica (
  id serial not null primary key,
  municipio varchar not null,
  uf char(2) not null
);

CREATE TABLE aposta (
  id serial8 not null primary key,
  id_sorteio int not null,
  id_loterica int not null,
  data_hora timestamp not null default now(),
  numeros int2[]
);
ALTER TABLE aposta ADD FOREIGN KEY id_sorteio ON sorteio (id);
ALTER TABLE aposta ADD FOREIGN KEY id_loterica ON loterica (id);

CREATE TABLE processamento (
  id int8 not null primary key,
  hash int8,
  acertos int2
);
ALTER TABLE processamento ADD FOREIGN KEY id ON aposta (id);

