import config from "common/configs/config.json" assert { type: "json" };

async function register(app, options)
{
    const LOGIN_SCHEMA =
    {
        summary: "Log into an account",
        query:
        {
            type: "object",
            required: [ "username", "password" ],
            additionalProperties: false,
            properties:
            {
                "username": { $ref: "username" },
                "password": { $ref: "password" }
            }
        },
        response:
        {
            200:
            {
                content:
                {
                    'application/json':
                    {
                        schema: { $ref: "http_ok" }
                    }
                },
                headers:
                {
                    "Set-Cookie": { type: "object", example: `__Secure-authorization=; Domain=${config.accounts.host}; Path=/; HttpOnly; SameSite=Lax` }
                }
            },
            default: { $ref: "http_error" }
        }
    };
    app.get("/login", { schema: LOGIN_SCHEMA }, async function(req, res)
    {
        if (await req.authorize()) await res.logout();
        await res.login(req.query.username, req.query.password);
        return res.send({ ok: true });
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, { name: 'routes', encapsulate: false });