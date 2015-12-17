#!/bin/bash

PSQL="/usr/bin/psql"
#PSQL="/usr/local/pgsql/bin/psql"

# verificar se diversos argumentos foram passados
if [ $# -gt 1 ]
then
  echo "Sintaxe: $0 [nproc]"
  echo "Exemplo: $0 8"
  exit 1
fi

# obter número de processos a serem abertos
if [ $# -eq 1 ]
then
  procs=$1
else
  procs=1
fi

if [ "$PGHOST" != "" ]; then echo "Considerando servidor: $PGHOST"; fi
if [ "$PGDATABASE" != "" ]; then echo "Considerando banco: $PGDATABASE"; fi
if [ "$PGUSER" != "" ]; then echo "Considerando usuário: $PGUSER"; fi

##########################################################################################

echo "Executando script com $procs processo(s)"
echo

inicio=`date`

# obter maior identificador de aposta
minid=`$PSQL -tA -c "SELECT min(id) FROM aposta WHERE id_concurso = 5"`
maxid=`$PSQL -tA -c "SELECT max(id) FROM aposta WHERE id_concurso = 5"`
qtdid=`$PSQL -tA -c "SELECT count(id) FROM aposta WHERE id_concurso = 5"`
echo "Limites => Mín: $minid, Máx: $maxid, Qtd: $qtdid"
echo

echo "Removendo índices e constraints da tabela de cálculo..."
echo "TRUNCATE calculo;
ALTER TABLE calculo DROP CONSTRAINT calculo_pkey;
ALTER TABLE calculo DROP CONSTRAINT calculo_id_fkey;
DROP INDEX calculo_acertos_idx;
" | $PSQL -q
echo

##########################################################################################

echo -e "[`date`]\n"

# calcular hash das apostas
echo "Calculando hash das apostas..."
echo

let chunks=procs*100000
sid=$minid
let eid=minid+chunks-1
let maxid+=chunks

# loop para cada processo
while [ $eid -le $maxid ]
do
  for nproc in `seq 1 $procs`
  do
    let resto=nproc-1
    sql="
INSERT INTO calculo (id, hash)
SELECT id, calcular_hash(numeros) AS hash
FROM aposta
WHERE id_concurso = 5
  AND id BETWEEN $sid AND $eid
  AND id % $procs = ${resto};
"
    #echo "=> Processo #$nproc [$sid->$eid]: $sql"
    $PSQL -q -c "$sql" &
  done
  #echo -e "[`date`]\n"
  wait
  let sid+=chunks
  let eid+=chunks
done

##########################################################################################

echo -e "[`date`]\n"

# calcular acertos de cada aposta
echo "Calculando acertos de cada aposta..."
echo

sid=$minid
let eid=minid+chunks-1

# loop para cada processo
while [ $eid -le $maxid ]
do
  for nproc in `seq 1 $procs`
  do
    let resto=nproc-1
    sql="
UPDATE calculo b
SET acertos = calcular_acertos(hash, hash_sorteio)
FROM aposta a JOIN concurso c ON (c.id = a.id_concurso)
WHERE b.id = a.id AND c.id = 5
  AND b.id BETWEEN $sid AND $eid
  AND b.id % $procs = ${resto};
"
    #echo "=> Processo #$nproc [$sid->$eid]: $sql"
    $PSQL -q -c "$sql" &
  done
  #echo -e "[`date`]\n"
  wait
  let sid+=chunks
  let eid+=chunks
done

##########################################################################################

echo -e "[`date`]\n"

echo "Recriando índices e constraints da tabela de cálculo..."
echo "ALTER TABLE calculo ADD CONSTRAINT calculo_pkey PRIMARY KEY (id);
ALTER TABLE calculo ADD CONSTRAINT calculo_id_fkey FOREIGN KEY (id) REFERENCES aposta (id);
CREATE INDEX calculo_acertos_idx ON calculo (acertos);
" | $PSQL -q
echo

##########################################################################################

echo "Concluído!"
termino=`date`

echo
echo "Iniciado às:   $inicio"
echo "Finalizado às: $termino"

