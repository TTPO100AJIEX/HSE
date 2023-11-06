import { Database } from "common/postgreSQL/postgreSQL.js";
import Interval from "common/utils/Interval.js";

async function register(app, options) {
	const POST_SLEEP_SCHEMA = {
		summary: "Add sleep data",
		body: {
			type: "object",
			required: ["duration"],
			additionalProperties: false,
			properties: {
				duration: {
					type: "string",
					pattern: Interval.format,
				},
			},
		},
		response: {
			200: { $ref: "http_ok" },
			default: { $ref: "http_error" },
		},
	};
	app.post("/sleep", { schema: POST_SLEEP_SCHEMA }, async function (req, res) {
		await Database.query(`INSERT INTO sleep(duration) VALUES ($1)`, [new Interval(req.body.duration).toPostgres()]);
		return res.send({ ok: true });
	});
}

import plugin from "fastify-plugin";
export default plugin(register, { name: "sleep_routes", encapsulate: false });
