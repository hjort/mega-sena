CREATE TABLE sorteio (
  id serial not null primary key,
  data_concurso date not null default current_date,
  valor_acumulado numeric(12,2),
  valor_pago numeric(12,2),
  numeros_sorteados int2[],
  ganhadores_sena int,
  premiacao_sena numeric(12,2),
  ganhadores_quina int,
  premiacao_quina numeric(12,2),
  ganhadores_quadra int,
  premiacao_quadra numeric(12,2)
);

CREATE TABLE loterica (
  id serial not null primary key,
  municipio varchar not null,
  uf char(2) not null
);

CREATE TABLE aposta (
  id serial8 not null primary key,
  id_sorteio int not null,
  id_loterica int not null,
  data_hora timestamp not null default now(),
  numeros int2[]
);
ALTER TABLE aposta ADD FOREIGN KEY id_sorteio ON sorteio (id);
ALTER TABLE aposta ADD FOREIGN KEY id_loterica ON loterica (id);

CREATE TABLE processamento (
  id int8 not null primary key,
  hash int8,
  acertos int2
);
ALTER TABLE processamento ADD FOREIGN KEY id ON aposta (id);

