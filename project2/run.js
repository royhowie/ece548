import DataSet from './dataset.js'

let pairs = [
    {
        file: 'data/vowels.txt',
        opt: { pos_class: '0', neg_class: '1', attributes: 13, delim: ' ' }
    }/*,
    {
        file: 'data/haberman.txt',
        opt: { pos_class: '1', neg_class: '2', attributes: 3, delim: ',' }
    },
    {
        file: 'data/transfusion.txt',
        opt: { pos_class: '1', neg_class: '0', attributes: 4, delim: ',' }
    }*/
]

pairs.forEach(set => {
    console.log('TESTING:' + set.file)
    for (let i = 1; i <= 7; i += 2) {
        console.log('NN=' + i)
        console.log('(random)')
        new DataSet(set.file, set.opt).load().test(i)
        console.log('(tomek_links)')
        new DataSet(set.file, set.opt).load().test_with_remove(i)
    }
})
