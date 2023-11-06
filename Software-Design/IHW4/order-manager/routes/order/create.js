import { OrdersDatabase } from 'common/postgreSQL/postgreSQL.js';

async function register(app, options)
{
    const CREATE_ORDER_SCHEMA =
    {
        summary: "Create a new order",
        body:
        {
            type: "object",
            required: [ "dishes" ],
            additionalProperties: false,
            properties:
            {
                "dishes":
                {
                    type: "array",
                    minItems: 1,
                    items: 
                    {
                        type: "object",
                        required: [ "id", "quantity" ],
                        additionalProperties: false,
                        properties:
                        {
                            "id": { $ref: "id" },
                            "quantity": { type: "integer", minimum: 1 }
                        }
                    }
                },
                "special_requests": { type: "string", default: "None" }
            }
        },
        security: [ { cookieAuth: [ ] } ],
        response:
        {
            200:
            {
                id: { $ref: "id" }
            },
            default: { $ref: "http_error" }
        }
    };
    app.post("/order", { schema: CREATE_ORDER_SCHEMA, config: { access: [ 'CUSTOMER', 'CHEF', 'MANAGER' ] } }, async function(req, res)
    {
        const transactionString = `DO LANGUAGE plpgsql $$ DECLARE order_id INT;
        BEGIN
            CREATE TEMPORARY TABLE to_update ON COMMIT DROP AS (SELECT dish_id::INT, quantity::INT FROM (VALUES %L) AS tmp(dish_id, quantity));

            UPDATE dishes SET quantity = dishes.quantity - to_update.quantity FROM to_update WHERE dishes.id = to_update.dish_id;
            INSERT INTO orders(user_id, special_requests) VALUES (%L, %L) RETURNING id INTO order_id;
            INSERT INTO order_dishes(order_id, dish_id, quantity) (SELECT order_id, dish_id, quantity FROM to_update);

            CREATE TEMPORARY TABLE return_value ON COMMIT DROP AS (SELECT order_id);
        END $$;`;
        const updateQuery = OrdersDatabase.format(transactionString, req.body.dishes.map(dish => [ dish.id, dish.quantity ]), req.authorization.id, req.body.special_requests);
        const [ _, { order_id } ] = await OrdersDatabase.query_multiple([ updateQuery, { query: `SELECT * FROM return_value;`, one_response: true } ]);
        return res.send({ id: order_id });
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, { name: 'routes', encapsulate: false });