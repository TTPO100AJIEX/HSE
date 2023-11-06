import config from "common/configs/config.json" assert { type: "json" };
import PostgreSQL from "common/postgreSQL/postgreSQL.js";

const Connection = new PostgreSQL({ ...config.orders.postgreSQL, database: "postgres" });
await Connection.query(Connection.format(`CREATE DATABASE %I TEMPLATE template0 ENCODING UTF8`, config.orders.postgreSQL.database));
console.info(`Created database ${config.orders.postgreSQL.database}`);
Connection.end();