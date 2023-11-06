import config from "common/configs/config.json" assert { type: "json" };

async function register(app, options)
{
    const LOGOUT_SCHEMA =
    {
        summary: "Logout",
        security: [ { cookieAuth: [ ] } ],
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
                    "Set-Cookie": { type: "object", example: `__Secure-authorization=abcdef.ghijkl; Domain=${config.accounts.host}; Path=/; HttpOnly; SameSite=Lax` }
                }
            },
            default: { $ref: "http_error" }
        }
    };
    app.get("/logout", { schema: LOGOUT_SCHEMA, config: { access: [ 'CUSTOMER', 'CHEF', 'MANAGER' ] } }, async function(req, res)
    {
        await res.logout();
        return res.send({ ok: true });
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, { name: 'routes', encapsulate: false });