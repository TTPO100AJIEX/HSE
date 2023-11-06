import { Database } from "common/postgreSQL/postgreSQL.js";
import Interval from "common/utils/Interval.js";

async function register(app, options) {
	const POST_NUTRITION_SCHEMA = {
		summary: "Add nutrition data",
		body: {
			type: "object",
			required: ["dish_name", "portion", "calories"],
			additionalProperties: false,
			properties: {
				dish_name: {
					type: "string",
				},
				portion: {
					type: "integer",
					exclusiveMinimum: 0,
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
	app.post("/nutrition", { schema: POST_NUTRITION_SCHEMA }, async function (req, res) {
		await Database.query(`INSERT INTO nutrition(dish_name, portion, calories) VALUES ($1, $2, $3)`, [
			req.body.dish_name,
			req.body.portion,
			req.body.calories,
		]);
		return res.send({ ok: true });
	});
}

import plugin from "fastify-plugin";
export default plugin(register, { name: "nutrition_routes", encapsulate: false });
