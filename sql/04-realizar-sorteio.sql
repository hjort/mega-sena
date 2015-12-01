-- efetuar o sorteio
UPDATE concurso
SET numeros_sorteados = sortear_numeros(1, 60, 6, 6)
WHERE id = 5;

-- calcular hash do sorteio
UPDATE concurso
SET hash_sorteio = calcular_hash(numeros_sorteados)
WHERE id = 5;

-- verificar dados do sorteio
SELECT id, numeros_sorteados, hash_sorteio
FROM concurso
WHERE id = 5;

