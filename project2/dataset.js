import fs from 'fs'
import AttrVector from './attr_vec.js'

export default class DataSet {
    constructor (fname, opt) {
        this.fname = fname
        this.options = opt
    }

    load () {
        let data = fs.readFileSync(this.fname, 'utf8').toString()
            .split('\n')
            .slice(0, -1)
            .map(line => new AttrVector(line, this.options))

        data = DataSet.shuffle(data)

        this.pos = []
        this.neg = []

        data.forEach(v => {
            if (v.class)
                this.pos.push(v)
            else
                this.neg.push(v)
        })

        this.l_class = this.s_class = 'pos'
        if (this.pos.length > this.neg.length) {
            this.s_class = 'neg'
        } else {
            this.l_class = 'neg'
        }

        return this
    }

    find_tomek_links () {
        let tomek_links = []
        let data_set = this.pos.concat(this.neg)

        data_set.forEach((v1, i) => {
            let nn = data_set
                .map((v2, j) => [v2, j, v2.distance(v1)])
                .sort((a, b) => a[2] - b[2])
                [2];

            if (i != nn[1] && v1.class != nn[0].class) {
                let p = [ v1, i ]
                let n = [ nn[0], nn[1] ]

                tomek_links.push({
                    pos: v1.class ? p : n,
                    neg: v1.class ? n : p
                })
            }
        })

        return tomek_links
    }

    nearest_neighbors_hypothesis (neighbor, amount, s_class_bound, l_class_bound) {
        let data_set = this[this.s_class].concat(
            this[this.l_class].slice(0, l_class_bound)
        )

        // 0. Record the distances from `neighbor`.
        // 1. Sort in ascending order.
        // 2. Ignore the first element as neighbor.distance(neighbor) == 0.
        let nn = data_set
            .map(v => [v, v.distance(neighbor)])
            .sort((a, b) => a[1] - b[1])
            .slice(1, 1 + amount)

        // Count the number of positive and negative neighbors
        // accum := [ pos_neighbors, neg_neighbors ]
        let score = nn.reduce((accum, next) => {
            if (next[0].class) accum[0]++
            else accum[1]++
            return accum
        }, [ 0, 0 ])

        return score[0] > score[1]
    }

    nn_hypothesis_with_filter (neighbor, amount = 1) {
        let distances = []

        this.pos.forEach(v => {
            if (v != null) distances.push([v, v.distance(neighbor)])
        })

        this.neg.forEach(v => {
            if (v != null) distances.push([v, v.distance(neighbor)])
        })

        let score = distances
            .sort((a, b) => a[1] - b[1])
            .slice(1, 1 + amount)
            .reduce((accum, next) => {
                if (next[0].class) accum[0]++
                else accum[1]++
                return accum
            }, [ 0, 0 ])

        return score[0] > score[1]
    }

    test (amount) {
        let ratio = this[this.l_class].length / this[this.s_class].length
        let rounds = 10 * (ratio - 1) | 0
        let s_class = this[this.s_class]
        let s_class_bound = s_class.length
        let l_class = this[this.l_class]
        let l_class_bound = l_class.length
        let delta = Math.ceil((l_class_bound - s_class_bound) / rounds)

        /*                  hypothesis
                            pos     neg
            label   pos     TP      FN
                    neg     FP      TN
        */
        console.log('ratio,,TP,FN,FP,TN')
        for (let i = 1; i <= rounds; i++) {
            let true_pos = 0, false_neg = 0, false_pos = 0, true_neg = 0

            for (let j = 0; j < s_class_bound; j++) {
                let hypothesis = this.nearest_neighbors_hypothesis(
                    s_class[j],
                    amount,
                    s_class_bound,
                    l_class_bound
                )

                if (hypothesis == true && s_class[j].class == true)
                    true_pos += 1
                else if (hypothesis == false && s_class[j].class == true)
                    false_neg += 1
                else if (hypothesis == true && s_class[j].class == false)
                    false_pos += 1
                else
                    true_neg += 1
            }

            for (let j = 0; j < l_class_bound; j++) {
                let hypothesis = this.nearest_neighbors_hypothesis(
                    l_class[j],
                    amount,
                    s_class_bound,
                    l_class_bound
                )

                if (hypothesis == true && l_class[j].class == true)
                    true_pos += 1
                else if (hypothesis == false && l_class[j].class == true)
                    false_neg += 1
                else if (hypothesis == true && l_class[j].class == false)
                    false_pos += 1
                else
                    true_neg += 1
            }

            let r = (l_class_bound / s_class_bound)
            console.log(`${r},,${true_pos},${false_neg},${false_pos},${true_neg}`)

            l_class_bound = Math.max(s_class_bound, l_class_bound - delta)
        }
    }

    test_with_remove (amount = 1) {
        let links = this.find_tomek_links()
        let len = links.length
        let s_class_len = this[this.s_class].length

        console.log('ratio,links_removed,TP,FN,FP,TN')
        while (links.length) {
            let true_pos = 0, false_neg = 0, false_pos = 0, true_neg = 0

            let cb = (v) => {
                if (v == null) return;
                let hypothesis = this.nn_hypothesis_with_filter(v, amount)

                if (hypothesis == true && v.class == true)
                    true_pos += 1
                else if (hypothesis == false && v.class == true)
                    false_neg += 1
                else if (hypothesis == true && v.class == false)
                    false_pos += 1
                else
                    true_neg += 1
            }

            this.pos.forEach(cb)
            this.neg.forEach(cb)

            let links_removed = (len - links.length)
            let l_class_len = this[this.l_class].length
            let ratio = (l_class_len - links_removed) / s_class_len
            console.log(`${ratio},${links_removed},${true_pos},${false_neg},${false_pos},${true_neg}`)

            links.splice(0, 10).forEach(link => {
                let over_rep = link[this.l_class]
                let index = over_rep[1]
                this[this.l_class][index] = null
            })
        }
    }

    // Fisher-Yates unbiased shuffle
    static shuffle (arr, start = 0, stop = arr.length) {
        let temp_val
        let random_index

        while (start !== stop) {
            random_index = Math.random() * (stop - start) | 0
            stop -= 1

            temp_val = arr[stop]
            arr[stop] = arr[random_index]
            arr[random_index] = temp_val
        }

        return arr
    }
}
