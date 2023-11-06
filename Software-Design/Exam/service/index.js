import config from "common/configs/config.json" assert { type: "json" };
import fastify from "fastify";
import ajv_formats from "ajv-formats";
import path from "path";
import url from "url";

const app = fastify({
	forceCloseConnections: true,
	ignoreTrailingSlash: true,
	ignoreDuplicateSlashes: true,
	logger: config.stage == "testing",
	disableRequestLogging: true,
	ajv: {
		plugins: [[ajv_formats, { mode: "full", keywords: true }]],
		customOptions: { removeAdditional: true, useDefaults: true, coerceTypes: "array" },
	},
});

import utility from "./plugins/utility.js";
await app.register(utility);
import security from "./plugins/security.js";
await app.register(security);
import request from "./plugins/request.js";
await app.register(request);
import response from "./plugins/response.js";
await app.register(response);
import routes from "./plugins/routes.js";
await app.register(routes, { directory: path.join(path.dirname(url.fileURLToPath(import.meta.url)), "routes") });

await app.ready();

app.listen({ port: config.port, host: "0.0.0.0" }, (err, address) => {
	if (err) throw err;
	console.info(`Server is now listening on ${address}`);
});
