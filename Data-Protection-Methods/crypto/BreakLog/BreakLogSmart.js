import find_divisors from "./utils/find_divisors.js";
import find_negative from "./utils/find_negative.js";
import pow from "./utils/pow.js";

export default function BreakLogSmart(q, b, y) // <b> = F*(q)
{
    let divisors = find_divisors(q - 1);
    let P = [ ], A = [ ];
    for (let i = 0; i < divisors.length; i++)
    {
        if (divisors[i] == P[P.length - 1]) A[A.length - 1]++;
        else { P.push(divisors[i]); A.push(1); }
    }
    let R = new Array(P.length);
    for (let i = 0; i < P.length; i++)
    {
        R[i] = new Map();
        let bp = BigInt(pow(b, (q - 1) / P[i], q)), current = 1n, bigq = BigInt(q);
        for (let j = 0; j < P[i]; j++)
        {
            R[i].set(Number(current), j);
            current = (current * bp) % bigq;
        }
    }
    
    let b_neg = find_negative(b, q);
    let answers = new Array(P.length);
    for (let t = 0; t < P.length; t++)
    {
        let X = new Array(A[t]);
        for (let i = 0; i < A[t]; i++)
        {
            let sum = 0, cur_pow = 1;
            for (let j = 0; j < i; j++) { sum += cur_pow * X[j]; cur_pow *= P[t]; }
            cur_pow *= P[t];
            let num = y * pow(b_neg, sum, q), power = (q - 1) / cur_pow, res = pow(num, power, q);
            X[i] = R[t].get(res);
        }
        let sum = 0, cur_pow = 1;
        for (let j = 0; j < X.length; j++) { sum += cur_pow * X[j]; cur_pow *= P[t]; }
        answers[t] = sum;
    }

    let M = q - 1, Mbig = BigInt(M);
    let Ms = P.map((value, index) => M / Math.pow(value, A[index]));
    let Ms_neg = Ms.map((m, index) => find_negative(m, Math.pow(P[index], A[index])));
    let sum = answers.reduce((prev, cur, index) => prev + Number((BigInt(cur) * BigInt(Ms[index]) * BigInt(Ms_neg[index])) % Mbig), 0);
    return sum % M;
};