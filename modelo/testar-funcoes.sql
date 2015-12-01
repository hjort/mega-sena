SET client_min_messages TO debug1;

-- function sortear_numeros(min_num int, max_num int, min_qtd int, max_qtd int): int2[]
SELECT sortear_numeros(1, 60, 6, 6) AS sorteio;
SELECT sortear_numeros(1, 60, 6, 15) AS aposta FROM generate_series(1, 10);

-- function calcular_hash(nums int2[]): int8
SELECT calcular_hash('{60,43,13,14,1,52}') AS hash_sorteio;
SELECT calcular_hash('{39,13,18,22,49,48,31,36}') AS hash_aposta;

-- function calcular_acertos(hash_sorteio int8, hash_aposta int8): int2
SELECT calcular_acertos(578716950163632129, 422522778685440);

---------------------------------------------------------------------------------------------------

SELECT sa.*, calcular_acertos(hash_sorteio, hash_aposta) AS acertos
FROM (
  SELECT sorteio.numeros AS numeros_sorteio, calcular_hash(sorteio.numeros) AS hash_sorteio,
    aposta.numeros AS numeros_aposta, calcular_hash(aposta.numeros) AS hash_aposta
  FROM (SELECT sortear_numeros(1, 60, 6, 6) AS numeros) sorteio,
   (SELECT sortear_numeros(1, 60, 6, 15) AS numeros) aposta
) sa;

---------------------------------------------------------------------------------------------------

SELECT calcular_hash(sortear_numeros(1, 60, 6, 6));
SELECT numeros_sorteados, calcular_hash(numeros_sorteados) FROM concurso WHERE id = 5;

