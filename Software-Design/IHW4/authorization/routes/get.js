async function register(app, options)
{
    const GET_SCHEMA =
    {
        summary: "Get user information",
        security: [ { cookieAuth: [ ] } ],
        response:
        {
            200:
            {
                type: "object",
                additionalProperties: false,
                properties:
                {
                    "id": { $ref: "id" },
                    "username": { $ref: "username" },
                    "role": { $ref: "user_role" }
                }
            },
            default: { $ref: "http_error" }
        }
    };
    app.get("/", { schema: GET_SCHEMA, config: { access: [ 'CUSTOMER', 'CHEF', 'MANAGER' ] } }, async function(req, res)
    {
        return res.send(await req.authorize());
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, { name: 'routes', encapsulate: false });