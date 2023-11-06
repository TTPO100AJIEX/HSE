import fs from "fs";
import path from "path";

async function register(app, options) {
	for (const filename of fs.readdirSync(options.directory)) {
		const fileStats = fs.lstatSync(path.join(options.directory, filename));
		if (fileStats.isDirectory()) await register(app, { directory: path.join(options.directory, filename) });
		if (!fileStats.isFile()) continue;
		const { default: module } = await import(path.join(options.directory, filename));
		app.register(module);
	}
	await app.after();
}

import plugin from "fastify-plugin";
export default plugin(register, { name: "routes", encapsulate: false });
