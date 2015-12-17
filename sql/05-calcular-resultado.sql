-- remover índices e constraints da tabela de cálculo
TRUNCATE calculo;
ALTER TABLE calculo DROP CONSTRAINT calculo_pkey;
ALTER TABLE calculo DROP CONSTRAINT calculo_id_fkey;
DROP INDEX calculo_acertos_idx;

\timing on

-- calcular hash das apostas
INSERT INTO calculo (id, hash)
SELECT id, calcular_hash(numeros) AS hash
FROM aposta
WHERE id_concurso = 5;

-- calcular acertos de cada aposta
UPDATE calculo b
SET acertos = calcular_acertos(hash, hash_sorteio)
FROM aposta a JOIN concurso c ON (c.id = a.id_concurso)
WHERE b.id = a.id AND c.id = 5;

\timing off

-- recriar índices e constraints da tabela de cálculo
ALTER TABLE calculo ADD PRIMARY KEY (id);
ALTER TABLE calculo ADD FOREIGN KEY (id) REFERENCES aposta (id);
CREATE INDEX ON calculo (acertos);


-- verificar dados inseridos

\dt+

SELECT a.id, numeros, hash, acertos
FROM aposta a JOIN calculo c ON (c.id = a.id)
WHERE id_concurso = 5
LIMIT 10;

