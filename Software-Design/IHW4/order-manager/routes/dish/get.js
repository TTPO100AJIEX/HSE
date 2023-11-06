import { OrdersDatabase } from 'common/postgreSQL/postgreSQL.js';

async function register(app, options)
{
    const GET_DISH_SCHEMA =
    {
        summary: "Get information about the dish",
        params:
        {
            type: "object",
            required: [ "id" ],
            additionalProperties: false,
            properties:
            {
                "id": { $ref: "id" }
            }
        },
        security: [ { cookieAuth: [ ] } ],
        response:
        {
            200:
            {
                type: "object",
                required: [ "id", "name", "description", "price", "quantity" ],
                additionalProperties: false,
                properties:
                {
                    id: { $ref: "id" },
                    name: { type: "string", maxLength: 100 },
                    description: { type: "string" },
                    price: { type: "number" },
                    quantity: { type: "integer", minimum: 1 }
                }
            },
            default: { $ref: "http_error" }
        }
    };
    app.get("/dish/:id", { schema: GET_DISH_SCHEMA, config: { access: [ 'MANAGER' ] } }, async function(req, res)
    {
        const dishData = await OrdersDatabase.query(`SELECT * FROM dishes WHERE id = $1`, [ req.params.id ], { one_response: true });
        if (!dishData) throw { statusCode: 404, message: `Dish with id ${req.params.id} does not exist` };
        return res.send(dishData);
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, { name: 'routes', encapsulate: false });