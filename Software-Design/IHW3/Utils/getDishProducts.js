export default function getDishProducts(obj) {
	let result = [];
	[].concat
		.apply(
			[],
			obj.operations.map(op => op.products)
		)
		.forEach(product => {
			if (!result.map(el => el.type).includes(product.type)) result.push({ type: product.type, quantity: product.quantity });
			else result.find(el => el.type === product.type).quantity += product.quantity;
		});
	return result;
}
