import config from "common/configs/config.json" assert { type: "json" };
import PostgreSQL from "common/postgreSQL/PostgreSQL.js";

const Database = new PostgreSQL({ ...config.postgreSQL, database: "postgres" });
await Database.query(Database.format(`DROP DATABASE %I WITH (FORCE)`, config.postgreSQL.database));
console.info(`Deleted database ${config.postgreSQL.database}`);
Database.end();
