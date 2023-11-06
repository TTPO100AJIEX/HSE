import config from "common/configs/config.json" assert { type: "json" };
import Interval from "common/utils/Interval/Interval.js";
import set_field_at_path from "common/utils/object_manipulation_by_path/set_field_at_path.js";

import pg from 'pg';
import format from 'pg-format';
pg.types.setTypeParser(pg.types.builtins.INTERVAL, value => new Interval(value));

export default class PostgreSQL
{
    static #DefaultOptions = { parseInputDatesAsUTC: true, application_name: config.application };
    static format = format;    
    format = format;

    #options;
    constructor(options) { this.#options = { ...PostgreSQL.#DefaultOptions, ...options }; }

    #raw_connection;
    get #connection()
    {
        if (this.#raw_connection) return this.#raw_connection;
        return this.#raw_connection = new pg.Pool(this.#options);
    }
    end() { if (this.#raw_connection) this.#raw_connection.end(); }
    get_raw() { return this.#connection; }

    #parse_response(rows)
    {
        for (const key in rows[0])
        {
            const path = key.split('_');
            if (path.length == 1) continue;
            for (let obj of rows) { const save = obj[key]; delete obj[key]; set_field_at_path(obj, path, save); }
        }
        return(rows);
    }

    async query(query, params, { parse = false, one_response = false } = { })
    {
        let data = await this.#connection.query(query, params);
        if (parse) data.rows = this.#parse_response(data.rows);
        if (one_response) data.rows = data.rows[0];
        return(data.rows);
    }
    async query_multiple(queries = { })
    {
        let final_query = "", plan = [ ];
        for (const key in queries)
        {
            const info = (typeof queries[key] !== "object") ? { query: queries[key] } : queries[key];
            plan.push({ name: key, parse: info.parse ?? false, one_response: info.one_response ?? false });
            final_query += `${info.query}${info.query.endsWith(";") ? "" : ";"}`;
        }
        if (plan.length == 0) return { };
        
        let data = await this.#connection.query(final_query);
        if (!Array.isArray(data)) data = [ data ];
        let result = Array.isArray(queries) ? [ ] : { };
        for (let i = 0; i < data.length; i++)
        {
            data[i] = data[i].rows;
            if (plan[i].parse) this.#parse_response(data[i]);
            if (plan[i].one_response) data[i] = data[i][0];
            result[plan[i].name] = data[i];
        }
        return(result);
    }
};

const AccountsDatabase = new PostgreSQL(config.accounts.postgreSQL);
const OrdersDatabase = new PostgreSQL(config.orders.postgreSQL);
export { AccountsDatabase, OrdersDatabase };