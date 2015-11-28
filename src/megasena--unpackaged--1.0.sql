/* contrib/megasena/megasena--unpackaged--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION megasena" to load this file. \quit

ALTER EXTENSION megasena ADD function sortear_numeros(min_num int, max_num int, min_qtd int, max_qtd int);

ALTER EXTENSION megasena ADD function calcular_hash(numeros varchar);

ALTER EXTENSION megasena ADD function calcular_acertos(hash_sorteio int8, hash_aposta int8);

