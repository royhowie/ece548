export default class AttrVector {
    constructor (line, opt) {
        let chunks = line.split(opt.delim).map(Number)

        this.data = chunks.slice(0, -1)
        this.class = chunks[opt.attributes] == opt.pos_class
    }

    distance (that) {
        if (!(that instanceof AttrVector))
            throw new Error('Must compare AttributeVector to AttributeVector!')

        return this.data
            .map((value, index) => Math.pow(value - that.data[index], 2))
            .reduce((accum, next) => accum + next)
    }

    toString () {
        return (this.class ? 'T' : 'F') + '\t' + this.data.join('\t')
    }
}
