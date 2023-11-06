import config from "common/configs/config.json" assert { type: "json" };
import PostgreSQL from "common/postgreSQL/postgreSQL.js";

const Connection = new PostgreSQL({ ...config.orders.postgreSQL, database: "postgres" });
await Connection.query(Connection.format(`DROP DATABASE %I WITH (FORCE)`, config.orders.postgreSQL.database));
console.info(`Deleted database ${config.orders.postgreSQL.database}`);
Connection.end();