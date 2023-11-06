import { Database } from "common/postgreSQL/postgreSQL.js";
import Interval from "common/utils/Interval.js";

async function register(app, options) {
	const POST_ACTIVITY_SCHEMA = {
		summary: "Add activity data",
		body: {
			type: "object",
			required: ["activity_type", "duration", "calories"],
			additionalProperties: false,
			properties: {
				activity_type: {
					type: "string",
				},
				duration: {
					type: "string",
					pattern: Interval.format,
				},
				calories: {
					type: "number",
				},
			},
		},
		response: {
			200: { $ref: "http_ok" },
			default: { $ref: "http_error" },
		},
	};
	app.post("/activity", { schema: POST_ACTIVITY_SCHEMA }, async function (req, res) {
		await Database.query(`INSERT INTO activity(activity_type, duration, calories) VALUES ($1, $2, $3)`, [
			req.body.activity_type,
			new Interval(req.body.duration).toPostgres(),
			req.body.calories,
		]);
		return res.send({ ok: true });
	});
}

import plugin from "fastify-plugin";
export default plugin(register, { name: "activity_routes", encapsulate: false });
