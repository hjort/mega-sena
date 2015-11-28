SET client_min_messages TO debug1;

-- function sortear_numeros(min_num int, max_num int, min_qtd int, max_qtd int): int2[]
SELECT sortear_numeros(1, 60, 6, 15);
SELECT sortear_numeros(1, 60, 6, 6);

-- function calcular_hash(nums int2[]): int8
--SET hash_sorteio = calcular_hash(numeros_sorteados)
--SELECT id, calcular_hash(numeros) AS hash

-- function calcular_acertos(hash_sorteio int8, hash_aposta int8): int2
SELECT calcular_acertos(63, 32);
SELECT calcular_acertos(63, 31);

