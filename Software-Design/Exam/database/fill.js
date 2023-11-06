import { Database } from "common/postgreSQL/postgreSQL.js";
import path from "path";
import url from "url";
import fs from "fs";

await Database.query(fs.readFileSync(path.join(path.dirname(url.fileURLToPath(import.meta.url)), "fill.sql"), "utf-8"));
console.info(`Finished database setup`);
Database.end();
