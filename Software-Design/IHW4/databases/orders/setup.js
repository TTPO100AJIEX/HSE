import { OrdersDatabase } from "common/postgreSQL/postgreSQL.js";
import path from 'path';
import url from 'url';
import fs from 'fs';

await OrdersDatabase.query(fs.readFileSync(path.join(path.dirname(url.fileURLToPath(import.meta.url)), "setup.sql"), "utf-8"));
console.info(`Created tables`);
OrdersDatabase.end();