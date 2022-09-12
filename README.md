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

function read_int(const table[], const column[], const here_column[], const here_value[]); // recuperar int em uma tabela especifica
function read_string(const column[], dest[]);// recuperar string em uma tabela especifica
function Float:read_float(const table[], const column[], const here_column[], const here_value[]); // recuperar float em uma tabela especifica
```
<br />
<b>Obs:</b> ainda incompleto! <b copyright="DeviceBlack">75%</b>
