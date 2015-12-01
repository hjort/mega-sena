\timing on

--TRUNCATE aposta, calculo;

-- inserir apostas

/*
INSERT INTO aposta (id_concurso, id_loterica, data_hora, numeros)
SELECT c.id AS id_concurso,
  ceil(random() * (SELECT max(id) FROM loterica)) AS id_loterica,
  c.data_concurso - (ceil(random() * 3 * 24 * 60) || ' minutes')::interval AS data_hora, -- 3 dias
  sortear_numeros(1, 60, 6, 15) AS numeros
FROM generate_series(1, 10000) a, concurso c
WHERE c.id = 5;
*/

INSERT INTO aposta (id_concurso, id_loterica, data_hora, numeros)
SELECT c.id AS id_concurso,
  ceil(random() * (SELECT max(id) FROM loterica)) AS id_loterica,
  c.data_concurso - (ceil(random() * 3 * 24 * 60) || ' minutes')::interval AS data_hora, -- 3 dias
  sortear_numeros(1, 60, n, n) AS numeros
FROM (
  SELECT n, generate_series(1, qtd) AS id
  FROM (
    SELECT n, round(1.0 / prop / 1.1996003996 * 100000)::int AS qtd
    FROM (
      SELECT n, ('{1,7,28,84,210,462,924,1716,3003,5005}'::int[])[n - 5] AS prop
      FROM generate_series(6, 15) n
    ) a
  ) b
) x, concurso c
WHERE c.id = 5;

/*
SELECT a.*, prop * 3.50 AS preco, round(1.0 / prop / 1.1996003996 * 100000) AS qtd_100k
FROM (
  SELECT n, ('{1,7,28,84,210,462,924,1716,3003,5005}'::int[])[n - 5] AS prop
  FROM generate_series(6, 15) n
) a;

SELECT n, 1.92222171462004E-6 * power(n, 8.5237608747) - 4.75 AS preco
FROM generate_series(6, 15) n
*/

\timing off


-- verificar dados inseridos

\dt+ aposta

SELECT * FROM aposta LIMIT 5;

SELECT array_length(numeros, 1) AS qtd_numeros, count(1) AS qtd_apostas
FROM aposta
GROUP BY qtd_numeros
ORDER BY qtd_numeros;

SELECT count(1) AS total FROM aposta;

