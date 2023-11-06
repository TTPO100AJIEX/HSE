import config from "common/configs/config.json" assert { type: "json" };

import helmet from "@fastify/helmet";
import rate_limit from "@fastify/rate-limit";

async function register(app, options) {
	/*----------------------------------HELMET----------------------------------*/
	await app.register(helmet, {
		global: true,
		enableCSPNonces: true,
		contentSecurityPolicy: false,
		crossOriginEmbedderPolicy: false,
		crossOriginOpenerPolicy: false,
		crossOriginResourcePolicy: false,
		originAgentCluster: true,
		referrerPolicy: { policy: "strict-origin-when-cross-origin" },
		strictTransportSecurity: { maxAge: 7 * 24 * 60 * 60, includeSubDomains: false, preload: true },
		xContentTypeOptions: true,
		dnsPrefetchControl: { allow: true },
		xFrameOptions: false,
	});

	/*----------------------------------RATE LIMIT----------------------------------*/
	await app.register(rate_limit, {
		global: true,
		max: config.stage == "testing" ? 1000000 : 100,
		timeWindow: 60000,
		ban: config.stage == "testing" ? 1500000 : 150,
		continueExceeding: true,
		addHeadersOnExceeding: { "x-ratelimit-limit": false, "x-ratelimit-remaining": false, "x-ratelimit-reset": false, "retry-after": false },
		addHeaders: { "x-ratelimit-limit": false, "x-ratelimit-remaining": true, "x-ratelimit-reset": false, "retry-after": true },
		onExceeded: req => console.info(`Rate Limit exceeded by ${req.ip}`),
	});
}

import plugin from "fastify-plugin";
export default plugin(register, { name: "security", encapsulate: false });
