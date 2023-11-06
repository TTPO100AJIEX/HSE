import config from "common/configs/config.json" assert { type: "json" };
import { AccountsDatabase } from "common/postgreSQL/postgreSQL.js";
import { AccountsCache } from "common/redis/redis.js";

import bcrypt from 'bcrypt';
import crypto from 'crypto';

import cookie from "@fastify/cookie";

async function register(app, options)
{
    app.addSchema({ $id: "id", type: "integer", minimum: -2147483648, maximum: 2147483647 });
    app.addSchema({ $id: "username", type: "string", maxLength: 50 });
    app.addSchema({ $id: "email", type: "string", format: "email" });
    app.addSchema({ $id: "password", type: "string", format: "password" });
    app.addSchema({ $id: "user_role", enum: [ 'CUSTOMER', 'CHEF', 'MANAGER' ] });

    await app.register(cookie, { secret: config.accounts.secret });
    // const COOKIE_NAME = '__Secure-authorization';
    // const COOKIE_OPTIONS = { domain: config.accounts.host, path: '/', secure: true, httpOnly: true, sameSite: 'Lax', signed: true };
    const COOKIE_NAME = 'Authorization';
    const COOKIE_OPTIONS = { domain: config.accounts.host, path: '/', secure: false, httpOnly: true, sameSite: 'Lax', signed: true };

    app.decorate("createAccount", async function(username, email, password, role)
    {
        password = await bcrypt.hash(password, config.accounts.bcrypt.saltRounds);
        await AccountsDatabase.query(`INSERT INTO users (username, email, password, role) VALUES ($1, $2, $3, $4)`, [ username, email, password, role ]);
    });

    app.decorateRequest("session_id", null);
    app.decorateRequest("get_sessionid", function()
    {
        if (this.session_id) return this.session_id;
        try {
            const session_id = this.unsignCookie(this.cookies[COOKIE_NAME]);
            if (session_id.valid && session_id.value.length == 44) return this.session_id = session_id.value;
            return this.session_id = null;
        } catch(err) { }
    });
    app.decorateRequest("create_sessionid", async function(data)
    {
        data.expiration = Date.now() + config.accounts.user_data_expiration;
        this.session_id = crypto.randomBytes(32).toString('base64');
        await AccountsCache.set_expire(`session_id-${this.session_id}`, JSON.stringify(data), config.accounts.authorization_expiration);
        return this.session_id;
    });
    app.decorateRequest("update_sessionid", async function(data)
    {
        data.expiration = Date.now() + config.accounts.user_data_expiration;
        await AccountsCache.set_keepttl(`session_id-${this.get_sessionid()}`, JSON.stringify(data));
    });
    app.decorateRequest("remove_sessionid", async function(data)
    {
        await AccountsCache.delete(`session_id-${this.get_sessionid()}`);
    });

    app.decorateRequest("authorization", null);
    app.decorateRequest("authorize", async function()
    {
        if (this.authorization) return this.authorization;
        if (!this.get_sessionid()) return this.authorization = null;
        let authorization = await AccountsCache.get_expire(`session_id-${this.get_sessionid()}`, config.accounts.authorization_expiration);
        if (!authorization) return this.authorization = null;
        authorization = JSON.parse(authorization);
        if (!authorization.expiration || new Date(authorization.expiration) <= new Date())
        {
            authorization = await AccountsDatabase.query(`SELECT id, username, role FROM users WHERE id = $1`, [ authorization.id ], { one_response: true });
            if (!authorization) { await req.remove_sessionid(); return this.authorization = null; }
            await this.update_sessionid(authorization);
        }
        return this.authorization = authorization;
    });

    app.addHook('preHandler', async (req, res) => 
    {
        if (!req.routeConfig?.access) return;
        const authorization = await req.authorize();
        if (!req.routeConfig.access.includes(authorization?.role || null)) throw 403;
    });

    app.decorateReply("login", async function(username, password)
    {
        const user = await AccountsDatabase.query(`SELECT id, username, password, role FROM users WHERE username = $1`, [ username ], { one_response: true });
        if (!user) throw { statusCode: 400, message: "Такого пользователя нет!" };
        if (!(await bcrypt.compare(password, user.password))) throw { statusCode: 401, message: "Пароль неверный!" };
        this.setCookie(COOKIE_NAME, await this.request.create_sessionid({ id: user.id, username: user.username, role: user.role }), COOKIE_OPTIONS);
    });

    app.decorateReply("logout", async function()
    {
        await this.request.remove_sessionid();
        this.clearCookie(COOKIE_NAME, COOKIE_OPTIONS);
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, { name: 'oauth', encapsulate: false });