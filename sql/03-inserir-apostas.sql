\timing on

--TRUNCATE aposta, calculo;

-- inserir apostas
INSERT INTO aposta (id_concurso, id_loterica, data_hora, numeros)
SELECT c.id AS id_concurso,
  ceil(random() * (SELECT max(id) FROM loterica)) AS id_loterica,
  c.data_concurso - (ceil(random() * 3 * 24 * 60) || ' minutes')::interval AS data_hora, -- 3 dias
  sortear_numeros(1, 60, 6, 15) AS numeros
FROM generate_series(1, 10000) a, concurso c
WHERE c.id = 5;

\timing off


-- verificar dados inseridos

\dt+ aposta

SELECT * FROM aposta LIMIT 5;

SELECT array_length(numeros, 1) AS qtd_numeros, count(1) AS qtd_apostas
FROM aposta
GROUP BY qtd_numeros
ORDER BY qtd_numeros DESC;

SELECT count(1) AS total FROM aposta;

