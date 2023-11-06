import config from "common/configs/config.json" assert { type: "json" };

import ioredis from 'ioredis';

export default class Redis
{
    static #DefaultOptions = { connectionName: config.application, enableAutoPipelining: true };

    #options;
    constructor(options) { this.#options = { ...Redis.#DefaultOptions, ...options }; }

    #raw_connection;
    get #connection()
    {
        if (this.#raw_connection) return this.#raw_connection;
        return this.#raw_connection = new ioredis(this.#options);
    }
    end() { if (this.#raw_connection) this.#raw_connection.quit(); }
    get_raw() { return this.#connection; }

    set(key, value) { return this.#connection.set(key, value); }
    set_expire(key, value, expiration) { return this.#connection.setex(key, expiration / 1000, value); }
    set_keepttl(key, value) { return this.#connection.set(key, value, "KEEPTTL"); }
    
    get(key) { return this.#connection.get(key); }
    get_delete(key) { return this.#connection.getdel(key); }
    async get_expire(key, expiration) { return (await this.#connection.multi().get(key).expire(key, expiration / 1000).exec())[0][1]; }

    delete(...keys) { return (keys.length == 0) ? [ ] : this.#connection.del(...keys); }
};

const AccountsCache = new Redis(config.accounts.redis);
const OrdersCache = new Redis(config.orders.redis);
export { AccountsCache, OrdersCache };