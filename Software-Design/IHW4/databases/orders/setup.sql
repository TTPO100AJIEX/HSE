CREATE TABLE dishes
(
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    description TEXT,
    price NUMERIC(10, 2) NOT NULL,
    quantity INT NOT NULL CHECK (quantity >= 0)
);




CREATE TYPE ORDER_STATUS AS ENUM ('WAITING', 'PROCESSING', 'COMPLETED', 'CANCELLED');
CREATE TABLE orders
(
    id INT GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    user_id INT NOT NULL,
    status ORDER_STATUS NOT NULL DEFAULT 'WAITING',
    special_requests TEXT,
    created_at TIMESTAMPTZ DEFAULT NOW(),
    updated_at TIMESTAMPTZ DEFAULT NOW()
);

CREATE FUNCTION synchronize_orders_updated_at() RETURNS TRIGGER
LANGUAGE plpgsql VOLATILE LEAKPROOF STRICT PARALLEL SAFE AS
$$ BEGIN
    NEW.created_at = OLD.created_at;
    NEW.updated_at = NOW();
	RETURN NEW;
END $$;
CREATE TRIGGER orders_updated_at_synchronization BEFORE UPDATE ON orders
FOR EACH ROW EXECUTE FUNCTION synchronize_orders_updated_at();




CREATE TABLE order_dishes
(
    order_id INT NOT NULL REFERENCES orders(id) MATCH FULL ON DELETE CASCADE ON UPDATE CASCADE,
    dish_id INT NOT NULL REFERENCES dishes(id) MATCH FULL ON DELETE CASCADE ON UPDATE CASCADE,
    quantity INT NOT NULL CHECK (quantity > 0),
    PRIMARY KEY (order_id, dish_id)
);