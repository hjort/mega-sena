-- function sortear_numeros(min_num int2, max_num int2, min_qtd int2, max_qtd int2): int2[]
sortear_numeros(1, 60, 6, 15) AS numeros
SET numeros_sorteados = sortear_numeros(1, 60, 6, 6)

-- function calcular_hash(nums int2[]): int8
SET hash_sorteio = calcular_hash(numeros_sorteados)
SELECT id, calcular_hash(numeros) AS hash

-- function calcular_acertos(hash_sorteio int8, hash_aposta int8): int2
SET acertos = calcular_acertos(hash, hash_sorteio)

