import config from "common/configs/config.json" assert { type: "json" };
import PostgreSQL from "common/postgreSQL/postgreSQL.js";

const Connection = new PostgreSQL({ ...config.accounts.postgreSQL, database: "postgres" });
await Connection.query(Connection.format(`DROP DATABASE %I WITH (FORCE)`, config.accounts.postgreSQL.database));
console.info(`Deleted database ${config.accounts.postgreSQL.database}`);
Connection.end();