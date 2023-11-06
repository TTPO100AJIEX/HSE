import config from "common/configs/config.json" assert { type: "json" };
import PostgreSQL from "common/postgreSQL/postgreSQL.js";

const Connection = new PostgreSQL({ ...config.accounts.postgreSQL, database: "postgres" });
await Connection.query(Connection.format(`CREATE DATABASE %I TEMPLATE template0 ENCODING UTF8`, config.accounts.postgreSQL.database));
console.info(`Created database ${config.accounts.postgreSQL.database}`);
Connection.end();