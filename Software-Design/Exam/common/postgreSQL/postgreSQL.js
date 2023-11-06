import config from "common/configs/config.json" assert { type: "json" };
import Interval from "common/utils/Interval.js";

import pg from "pg";
import pg_format from "pg-format";
import JSONPointer from "jsonpointer";

pg.types.setTypeParser(pg.types.builtins.INTERVAL, value => new Interval(value));

export default class PostgreSQL {
	static #default_options = { parseInputDatesAsUTC: true, application_name: config.application.name };
	format = pg_format;

	#options;
	#pg_connection;
	constructor(options) {
		this.#options = { ...PostgreSQL.#default_options, ...options };
	}
	get #connection() {
		if (!this.#pg_connection) {
			this.#pg_connection = new pg.Pool(this.#options);
			console.info(`Connected to PostgreSQL database ${this.#options.database}`);
		}
		return this.#pg_connection;
	}
	end() {
		if (this.#pg_connection) this.#pg_connection.end();
		this.#pg_connection = null;
	}
	getRaw() {
		return this.#connection;
	}

	#parsePointers(data) {
		if (typeof data != "object" || !data || data instanceof Interval) return data;
		let result = Array.isArray(data) ? [] : {};
		for (const key in data) JSONPointer.set(result, "/" + key, this.#parsePointers(data[key]));
		return result;
	}

	async query(query, params, { one_response = false } = {}) {
		const data = await this.#connection.query(query, params);
		const rows = this.#parsePointers(data.rows);
		return one_response ? rows[0] : rows;
	}

	async queryMultiple(queries = {}) {
		const plan = Object.entries(queries).map(entry => {
			const record = {
				name: entry[0],
				query: entry[1].query ?? entry[1],
				one_response: entry[1].one_response ?? false,
			};
			if (record.query.includes(";")) console.warn(`PostgreSQL::query_multiple - semicolon detected in query ${record.query}`);
			return record;
		});
		if (plan.length == 0) return [];
		const data = await this.#connection.query(plan.map(record => record.query).join(";"));
		const rows = this.#parsePointers((Array.isArray(data) ? data : [data]).map(res => res.rows));
		const result = Array.isArray(queries) ? [] : {};
		for (let i = 0; i < plan.length; i++) result[plan[i].name] = plan[i].one_response ? rows[i][0] : rows[i];
		return result;
	}
}

export const Database = new PostgreSQL(config.postgreSQL);
