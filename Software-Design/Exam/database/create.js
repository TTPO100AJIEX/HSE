import config from "common/configs/config.json" assert { type: "json" };
import PostgreSQL from "common/postgreSQL/PostgreSQL.js";

const Database = new PostgreSQL({ ...config.postgreSQL, database: "postgres" });
await Database.query(Database.format(`CREATE DATABASE %I TEMPLATE template0 ENCODING UTF8`, config.postgreSQL.database));
console.info(`Created database ${config.postgreSQL.database}`);
Database.end();
