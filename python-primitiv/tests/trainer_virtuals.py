from primitiv import trainers as T
from primitiv import Trainer

import unittest
import tempfile


class TrainerVirtualFuncTest(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        pass

    @classmethod
    def tearDownClass(cls):
        pass

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_sgd_virtual(self):
        t = T.SGD()
        self.assertEqual(t.name(), "SGD")
        uint_configs = {'Trainer.epoch': 1}
        float_configs = {'SGD.eta': 0.0,
                         'Trainer.clip_threshold': 0.0,
                         'Trainer.lr_scale': 1.0,
                         'Trainer.l2_strength': 0.0,
        }
        t.set_configs(uint_configs, float_configs)
        uint_configs, float_configs = t.get_configs()
        self.assertEqual(uint_configs['Trainer.epoch'], 1)

    def test_momentum_sgd_virtual(self):
        t = T.MomentumSGD()
        self.assertEqual(t.name(), "MomentumSGD")
        uint_configs = {'Trainer.epoch': 1}
        float_configs = {'MomentumSGD.momentum': 1.0,
                         'MomentumSGD.eta': 0.0,
                         'Trainer.clip_threshold': 0.0,
                         'Trainer.lr_scale': 1.0,
                         'Trainer.l2_strength': 0.0,
        }
        t.set_configs(uint_configs, float_configs)
        uint_configs, float_configs = t.get_configs()
        self.assertEqual(uint_configs['Trainer.epoch'], 1)

    def test_adagrad_virtual(self):
        t = T.AdaGrad()
        self.assertEqual(t.name(), "AdaGrad")
        uint_configs = {'Trainer.epoch': 1}
        float_configs = {'AdaGrad.eps': 0.0,
                         'AdaGrad.eta': 0.0,
                         'Trainer.clip_threshold': 0.0,
                         'Trainer.lr_scale': 1.0,
                         'Trainer.l2_strength': 0.0,
        }
        t.set_configs(uint_configs, float_configs)
        uint_configs, float_configs = t.get_configs()
        self.assertEqual(uint_configs['Trainer.epoch'], 1)

    def test_rmsprop_virtual(self):
        t = T.RMSProp()
        self.assertEqual(t.name(), "RMSProp")
        uint_configs = {'Trainer.epoch': 1}
        float_configs = {'RMSProp.eta': 2.0,
                         'RMSProp.alpha': 3.0,
                         'RMSProp.eps': 4.0,
                         'Trainer.clip_threshold': 0.0,
                         'Trainer.lr_scale': 1.0,
                         'Trainer.l2_strength': 0.0,
        }
        t.set_configs(uint_configs, float_configs)
        uint_configs, float_configs = t.get_configs()
        self.assertEqual(uint_configs['Trainer.epoch'], 1)

    def test_adadelta_virtual(self):
        t = T.AdaDelta()
        self.assertEqual(t.name(), "AdaDelta")
        uint_configs = {'Trainer.epoch': 1}
        float_configs = {'AdaDelta.rho': 2.0,
                         'AdaDelta.eps': 3.0,
                         'Trainer.clip_threshold': 0.0,
                         'Trainer.lr_scale': 1.0,
                         'Trainer.l2_strength': 0.0,
        }
        t.set_configs(uint_configs, float_configs)
        uint_configs, float_configs = t.get_configs()
        self.assertEqual(uint_configs['Trainer.epoch'], 1)

    def test_adam_virtual(self):
        t = T.Adam()
        self.assertEqual(t.name(), "Adam")
        uint_configs = {'Trainer.epoch': 1}
        float_configs = {'Trainer.lr_scale': 1.0,
                         'Adam.beta2': 1.0,
                         'Adam.eps': 0.0,
                         'Trainer.clip_threshold': 0.0,
                         'Adam.alpha': 0.0,
                         'Trainer.l2_strength': 0.0,
                         'Adam.beta1': 1.0,
        }
        t.set_configs(uint_configs, float_configs)
        uint_configs, float_configs = t.get_configs()
        self.assertEqual(uint_configs['Trainer.epoch'], 1)