(1)
policy_kwargs = dict(act_fun=tf.nn.tanh, net_arch=[128,64,dict(vf=[32], pi=[32])])
env = DummyVecEnv([self.make_env(config_dict["TrainDataPath"],config_dict["PreDataType"]) for i in range(4)])
self.model = PPO2(MlpPolicy, env,policy_kwargs=policy_kwargs,verbose=1,n_cpu_tf_sess=4)
(2)
{
    "ModelName": "../../tmp/DAT_MT_EURUSD_M1_2015_CLOSE_DIFF",
    "Version": "202003031545",
    "Timer_SaveAlgrithmPath": "../../tmp/Timer_DAT_MT_EURUSD_M1_2015_CLOSE_DIFF_Algrithm.pkl",
    "Best_SaveAlgrithmPath": "../../tmp/Best_DAT_MT_EURUSD_M1_2015_CLOSE_DIFF_Algrithm.pkl",
    "PreDataType": "difflization",
    "TrainDataPath": "data/DAT_MT_EURUSD_M1_2015_CLOSE_train.csv",
    "TestDataPath": "data/DAT_MT_EURUSD_M1_2015_CLOSE_test.csv",
    "LearnBatchTimes": 10000,
    "LearnEachTimesteps": 100000,
    "LearnTatalTimesteps": 10000000000.0,
    "LogDir": "../../tmp/",
    "CurrentMean": -2019.4285714285713
}
