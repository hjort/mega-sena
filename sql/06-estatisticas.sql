-- resultados e estatísticas do concurso

SELECT count(1), min(acertos), max(acertos),
  trunc(avg(acertos),2) AS avg,
  trunc(stddev(acertos),2) AS stddev
FROM calculo;

SELECT acertos, count(1) AS qtd
FROM calculo
GROUP BY acertos
ORDER BY acertos desc;

SELECT a.id, municipio, uf, data_hora, numeros, acertos
FROM calculo c
  JOIN aposta a ON (a.id = c.id)
  JOIN loterica l ON (l.id = a.id_loterica)
WHERE acertos = (SELECT max(acertos) FROM calculo);

