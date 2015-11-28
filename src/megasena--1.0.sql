/* contrib/megasena/megasena--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION megasena" to load this file. \quit

CREATE FUNCTION sortear_numeros(min_num int, max_num int, min_qtd int, max_qtd int)
	RETURNS int2[]
	AS 'MODULE_PATHNAME', 'pg_sortear_numeros'
	LANGUAGE C
	IMMUTABLE STRICT;

CREATE FUNCTION calcular_hash(numeros int2[])
	RETURNS int8
	AS 'MODULE_PATHNAME', 'pg_calcular_hash'
	LANGUAGE C
	IMMUTABLE STRICT;

CREATE FUNCTION calcular_acertos(hash_sorteio int8, hash_aposta int8)
	RETURNS int2
	AS 'MODULE_PATHNAME', 'pg_calcular_acertos'
	LANGUAGE C
	IMMUTABLE STRICT;

