import { OrdersDatabase } from 'common/postgreSQL/postgreSQL.js';

async function register(app, options)
{
    const CREATE_DISH_SCHEMA =
    {
        summary: "Create a dish",
        body:
        {
            type: "object",
            required: [ "name", "description", "price", "quantity" ],
            additionalProperties: false,
            properties:
            {
                name: { type: "string", maxLength: 100 },
                description: { type: "string" },
                price: { type: "number" },
                quantity: { type: "integer", minimum: 1 }
            }
        },
        security: [ { cookieAuth: [ ] } ],
        response:
        {
            200:
            {
                "id": { $ref: "id" }
            },
            default: { $ref: "http_error" }
        }
    };
    app.post("/dish", { schema: CREATE_DISH_SCHEMA, config: { access: [ 'MANAGER' ] } }, async function(req, res)
    {
        const queryString = `INSERT INTO dishes(name, description, price, quantity) VALUES ($1, $2, $3, $4) RETURNING id`;
        const queryParams = [ req.body.name, req.body.description, req.body.price, req.body.quantity ];
        return res.send(await OrdersDatabase.query(queryString, queryParams, { one_response: true }));
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, { name: 'routes', encapsulate: false });