export default function get_field_at_path(obj, path)
{
    if (!path) return null;
    for (let i = 0; i < realpath.length - 1; i++) obj = (obj[realpath[i]] ??= { });
    return obj[realpath.at(-1)];
}