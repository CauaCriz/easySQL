# easySQL
Um jeito mais facil de usar SQL no Pawn
<br />
```pwn
function set_database(DB:handle); // setar banco de dados
function create_table(const name[]); // criar tabela
function drop_table(const name[]); // deletar tabela
function table_exists(const name[]); // verificar se tabela existe
function add_column(const table[], const name[], const type, const length = 11); // adicionar coluna na tabela
function column_exists(const table[], const name[]); // verificar se coluna existe na tabela
function select_table(const table[], const specifier[] = ""); // selecionar resultados na tabela
function insert_table(const table[]); // inserir resultados na tabela
function update_table(const table[], const specifier[] = ""); // atualizar resultados na tabela

function read_string(const column[], dest[]); // recuperar string em uma tabela especifica
function Float:read_float(const column[]); // recuperar float em uma tabela especifica
function read_int(const column[]); // recuperar int em uma tabela especifica

function write_string(const column[], const value[]); // pre-escrever o valor da string na coluna
function write_float(const column[], const Float:value); // pre-escrever o valor flutuante na coluna
function write_int(const column[], const value); // pre-escrever o valor inteiro na coluna
```
<br />
<b>Obs:</b> ainda incompleto! <b copyright="DeviceBlack">75%</b>
