import { Database } from "common/postgreSQL/postgreSQL.js";
import Interval from "common/utils/Interval.js";

async function register(app, options) {
	const GET_STATS_SCHEMA = {
		summary: "Get health stats",
		response: {
			200: {
				calories_received: {
					type: "number",
				},
				calories_lost: {
					type: "number",
				},
				activity_duration: {
					type: "string",
					pattern: Interval.format,
				},
				sleep_duration: {
					type: "string",
					pattern: Interval.format,
				},
			},
			default: { $ref: "http_error" },
		},
	};
	app.get("/stats", { schema: GET_STATS_SCHEMA }, async function (req, res) {
		const [{ calories_received }, { calories_lost }, { activity_duration }, { sleep_duration }] = await Database.queryMultiple([
			{ query: `SELECT SUM(calories) AS calories_received FROM nutrition`, one_response: true },
			{ query: `SELECT SUM(calories) AS calories_lost FROM activity`, one_response: true },
			{ query: `SELECT SUM(duration) AS activity_duration FROM activity`, one_response: true },
			{ query: `SELECT SUM(duration) AS sleep_duration FROM sleep`, one_response: true },
		]);
		return res.send({
			calories_received,
			calories_lost,
			activity_duration: activity_duration.toFormat(),
			sleep_duration: sleep_duration.toFormat(),
		});
	});
}

import plugin from "fastify-plugin";
export default plugin(register, { name: "sleep_routes", encapsulate: false });
