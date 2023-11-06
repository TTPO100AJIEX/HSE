async function register(app, options)
{
    const CREATE_SCHEMA =
    {
        summary: "Create a new account",
        body:
        {
            type: "object",
            required: [ "username", "email", "password", "role" ],
            additionalProperties: false,
            properties:
            {
                "username": { $ref: "username" },
                "email": { $ref: "email" },
                "password": { $ref: "password" },
                "role": { $ref: "user_role" }
            }
        },
        response:
        {
            200: { $ref: "http_ok" },
            default: { $ref: "http_error" }
        }
    };
    app.post("/create", { schema: CREATE_SCHEMA }, async function(req, res)
    {
        await app.createAccount(req.body.username, req.body.email, req.body.password, req.body.role);
        return res.send({ ok: true });
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, { name: 'routes', encapsulate: false });