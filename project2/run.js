import DataSet from './dataset.js'


if (true) {
    /*
        patient_age, operation_year (i.e., year - 1900), axillary nodes detected
        class: 1 if patient survived 5+ years, 0 if patient died within 5 years
    */
    let data_set = new DataSet('data/haberman.txt', {
        pos_class : '1',
        neg_class : '2',
        attributes: 3,
        delim     : ',',
    })

    data_set.load().test(1)
} else {
    /*
        recency(months), frequency (times), monetary (c.c. blood), time (months)
        outcome: whether (s)he donated blood in March 2007 (binary class)
    */
    let data_set = new DataSet('data/transfusion.txt', {
        pos_class : '1',
        neg_class : '0',
        attributes: 4,
        delim     : ',',
    })

    data_set.load().test(3)
}
