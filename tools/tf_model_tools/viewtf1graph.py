import sys
import tensorflow.compat.v1 as tf
# import tensorflow as tf
from log_init import log_init
import logging


from absl import app
from absl import flags
from absl import logging

import numpy as np

# import dataloader
# import det_model_fn
# import hparams_config
# import utils

# Cloud TPU Cluster Resolvers
flags.DEFINE_string('input_serve_model_dir', default=None,help='input serve model dir')
flags.DEFINE_string('input_simple_model_dir', default=None,help='input simple model dir')
flags.DEFINE_string('output_dir', default=None,help='output dir')
FLAGS = flags.FLAGS

def view_effiect_bykaras():
    # # new_model = tf.keras.models.load_model('/home/sailingzhang/tmp/myraccoon_savemodel')
    # new_model = tf.keras.models.load_model('/home/sailingzhang/tmp/saved_model')
    # # new_model.summary()
    # new_model.summary()
    pass


def view_effiect_tf1():
    sess=tf.Session
    with tf.Graph().as_default():
        with tf.gfile.FastGFile('/home/sailingzhang/tmp/myraccoon_savemodel/saved_model.pb','rb') as modelfile:
            graph_def=tf.GraphDef()
            graph_def.ParseFromString(modelfile.read())
            tf.import_graph_def(graph_def)
            [print(n.name) for n in tf.get_default_graph().as_graph_def().node]




def view_graph_tf2(output_graph_path):
    tf.reset_default_graph()  # 重置计算图
    with tf.Session() as sess:
        tf.global_variables_initializer().run()
        output_graph_def = tf.GraphDef()
        # 获得默认的图
        graph = tf.get_default_graph()
        with open(output_graph_path, "rb") as f:
            output_graph_def.ParseFromString(f.read())
            _ = tf.import_graph_def(output_graph_def, name="")
            # 得到当前图有几个操作节点
            print("%d ops in the final graph." % len(output_graph_def.node))

            tensor_name = [tensor.name for tensor in output_graph_def.node]
            print(tensor_name)
            print('---------------------------')
            # 在log_graph文件夹下生产日志文件，可以在tensorboard中可视化模型
            summaryWriter = tf.summary.FileWriter('log_pb/', graph)

            for op in graph.get_operations():
                # print出tensor的name和值
                print(op.name, op.values())

def view_graph_serve_bydir(pathdir):
    with tf.Session(graph=tf.Graph()) as sess:
        tf.saved_model.loader.load(sess, ["serve"], pathdir)
        graph = tf.get_default_graph()
        [print(n.name) for n in tf.get_default_graph().as_graph_def().node]
        summaryWriter = tf.summary.FileWriter(FLAGS.output_dir, graph)


def save_as_pb(sess,output_node_names,pbfile):
    if os.path.exists(pbfile):
        os.remove(pbfile)
    graph = sess.graph
    # graph = tf.get_default_graph()
    input_graph_def = graph.as_graph_def()
    # output_node_names = ['cnn/output']

    output_graph_def = graph_util.convert_variables_to_constants(sess, input_graph_def, output_node_names)

    with tf.gfile.GFile(pbfile, 'wb') as f:
        f.write(output_graph_def.SerializeToString())
    
    return pbfile


if __name__ == "__main__":        
    logging.basicConfig(filename=output_dir+"/viewtf1graph.log",filemode="w",format="%(asctime)s-%(name)s-%(levelname)s-%(message)s",level=logging.DEBUG)
    logging.info("enter")
    if FLAGS.input_serve_model_dir is not None:
        
    else:

    # view_effiect_bykaras()
    # view_effiect_tf1()
    # pbpath="/home/sailingzhang/winshare/develop/source/TONGWEI_SVN2/gentoo/trunk/products/cognition/facerecognition/embed_model/20180402-114759.pb"
    pbpath="/home/sailingzhang/tmp/myraccoon_savemodel/saved_model.pb"
    # view_graph_tf2(pbpath)
    view_graph_bydir("/home/sailingzhang/tmp/myraccoon_savemodel")
    logging.info("exit")
