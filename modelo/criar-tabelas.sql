DROP TABLE IF EXISTS concurso CASCADE;
CREATE TABLE concurso (
  id serial not null primary key,
  data_concurso date not null default current_date,
  valor_acumulado numeric(12,2),
  valor_pago numeric(12,2),
  numeros_sorteados int2[],
  hash_sorteio int8,
  ganhadores_sena int,
  premiacao_sena numeric(12,2),
  ganhadores_quina int,
  premiacao_quina numeric(12,2),
  ganhadores_quadra int,
  premiacao_quadra numeric(12,2)
);

DROP TABLE IF EXISTS loterica CASCADE;
CREATE TABLE loterica (
  id serial not null primary key,
  municipio varchar not null,
  uf char(2) not null
);

DROP TABLE IF EXISTS aposta CASCADE;
CREATE TABLE aposta (
  id serial8 not null primary key,
  id_concurso int not null,
  id_loterica int not null,
  data_hora timestamp not null default now(),
  numeros int2[]
);
ALTER TABLE aposta ADD FOREIGN KEY (id_concurso) REFERENCES concurso (id);
ALTER TABLE aposta ADD FOREIGN KEY (id_loterica) REFERENCES loterica (id);

DROP TABLE IF EXISTS calculo CASCADE;
CREATE TABLE calculo (
  id int8 not null primary key,
  hash int8,
  acertos int2
);
ALTER TABLE calculo ADD FOREIGN KEY (id) REFERENCES aposta (id);

