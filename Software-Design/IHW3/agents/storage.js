import { StorageServer } from "utils/grpc/server.js";

class Storage extends StorageServer {
	#startTimestamp = Date.now();
	#runTime() {
		return Date.now() - this.#startTimestamp;
	}

	constructor(port, productsPath) {
		super(port);
		this.#loadProducts(productsPath).catch(err => {
			console.error(err);
			process.exit(-1);
		});
	}
	#products;
	async #loadProducts(productsPath) {
		this.#products = (await import("../data/" + productsPath, { assert: { type: "json" } })).default;
		// TODO: wtf is console.log!!! Use throw
		this.#products.forEach(product => {
			if (!("id" in product)) console.log("no id in product");
			if (product.id < 0) console.log("incorrect product id");

			if (!("type" in product)) console.log("no type in product");
			if (product.type < 0) console.log("incorrect product type");

			if (!("name" in product)) console.log("no name in product");
			if (product.name.length === 0) console.log("incorrect product name");

			if (!("company" in product)) console.log("no company in product");
			if (product.company.length === 0) console.log("incorrect product company");

			if (!("cost" in product)) console.log("no cost in product");
			if (product.cost < 0) console.log("incorrect product cost");

			if (!("quantity" in product)) console.log("no quantity in product");
			if (product.quantity < 0) console.log("incorrect product quantity");

			if (!("unit" in product)) console.log("no unit in product");
			if (product.unit.length === 0) console.log("incorrect product unit");

			if (!("delivered" in product)) console.log("no delivered in product");

			if (!("valid_until" in product)) console.log("no valid_until in product");
			if (product.valid_until < 0) console.log("incorrect product valid_until");
		});
		console.log("Operations have been loaded succesfully");
	}

	async getAvailable(data, callback) {
		callback(null, {
			products: this.#products.filter(el => {
				return el.quantity !== 0; // TODO: expiration date counting
			}),
		});
	}

	async getProducts(data, callback) {
		// TODO: check if requested products are available.
		// If any of the products is not available, send { ok: false }
		// Otherwise, send { ok: true, products: [ ... ] } and adjust this.#products (remove the requested amounts)
		// !!! this.#products should not be changed if { ok: false }
		callback(null, { ok: true, products: [] });
	}
}

const storage = new Storage(process.argv[2], process.argv[3]);
