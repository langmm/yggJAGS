def scalar(param):
    return param

def vector(param):
    import numpy as np
    return np.array([param.sum()])

def array(param):
    import numpy as np
    return np.array([param.sum()])

def array_to_array(param):
    return 3 * param
