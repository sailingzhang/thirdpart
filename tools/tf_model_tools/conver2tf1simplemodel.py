import os
import sys
import tensorflow.compat.v1 as tf
from tensorflow.python.framework import graph_util
# import tensorflow as tf

import logging







def load_model(model, input_map=None):
    # Check if the model is a model directory (containing a metagraph and a checkpoint file)
    #  or if it is a protobuf file with a frozen graph
    model_exp = os.path.expanduser(model)
    if (os.path.isfile(model_exp)):
        print('Model filename: %s' % model_exp)
        with gfile.FastGFile(model_exp,'rb') as f:
            graph_def = tf.GraphDef()
            graph_def.ParseFromString(f.read())
            tf.import_graph_def(graph_def, input_map=input_map, name='')
    else:
        print('Model directory: %s' % model_exp)
        meta_file, ckpt_file = get_model_filenames(model_exp)
        
        print('Metagraph file: %s' % meta_file)
        print('Checkpoint file: %s' % ckpt_file)
      
        saver = tf.train.import_meta_graph(os.path.join(model_exp, meta_file), input_map=input_map)
        saver.restore(tf.get_default_session(), os.path.join(model_exp, ckpt_file))
    
def get_model_filenames(model_dir):
    files = os.listdir(model_dir)
    meta_files = [s for s in files if s.endswith('.meta')]
    if len(meta_files)==0:
        raise ValueError('No meta file found in the model directory (%s)' % model_dir)
    elif len(meta_files)>1:
        raise ValueError('There should not be more than one meta file in the model directory (%s)' % model_dir)
    meta_file = meta_files[0]
    ckpt = tf.train.get_checkpoint_state(model_dir)
    if ckpt and ckpt.model_checkpoint_path:
        ckpt_file = os.path.basename(ckpt.model_checkpoint_path)
        return meta_file, ckpt_file

    meta_files = [s for s in files if '.ckpt' in s]
    max_step = -1
    for f in files:
        step_str = re.match(r'(^model-[\w\- ]+.ckpt-(\d+))', f)
        if step_str is not None and len(step_str.groups())>=2:
            step = int(step_str.groups()[1])
            if step > max_step:
                max_step = step
                ckpt_file = step_str.groups()[0]
    return meta_file, ckpt_file





def freeze_graph(input_checkpoint,output_graph):
    '''
    :param input_checkpoint:
    :param output_graph: PB模型保存路径
    :return:
    '''
    # checkpoint = tf.train.get_checkpoint_state(model_folder) #检查目录下ckpt文件状态是否可用
    # input_checkpoint = checkpoint.model_checkpoint_path #得ckpt文件路径
 
    # 指定输出的节点名称,该节点名称必须是原模型中存在的节点
    output_node_names = "InceptionV3/Logits/SpatialSqueeze"
    saver = tf.train.import_meta_graph(input_checkpoint + '.meta', clear_devices=True)
    graph = tf.get_default_graph() # 获得默认的图
    input_graph_def = graph.as_graph_def()  # 返回一个序列化的图代表当前的图
 
    with tf.Session() as sess:
        saver.restore(sess, input_checkpoint) #恢复图并得到数据
        output_graph_def = graph_util.convert_variables_to_constants(  # 模型持久化，将变量值固定
            sess=sess,
            input_graph_def=input_graph_def,# 等于:sess.graph_def
            output_node_names=output_node_names.split(","))# 如果有多个输出节点，以逗号隔开
 
        with tf.gfile.GFile(output_graph, "wb") as f: #保存模型
            f.write(output_graph_def.SerializeToString()) #序列化输出
        print("%d ops in the final graph." % len(output_graph_def.node)) #得到当前图有几个操作节点



def  convertToServerModel(model_path):
    with tf.Graph().as_default():
        sess = tf.Session()
        with sess.as_default():
            logging.info("really loading emb")
            facenet.load_model(self.modelpath)
            self.sess = sess
            # images_placeholder = tf.get_default_graph().get_tensor_by_name("input:0")
            # embeddings = tf.get_default_graph().get_tensor_by_name("embeddings:0")
            # phase_train_placeholder = tf.get_default_graph().get_tensor_by_name("phase_train:0")
            # tf.summary.FileWriter('graphlog1/', self.sess.graph)
            def embed_base(aligned_img_list):
                logging.debug("begin embed,aligned_emd_list'len={}".format(len(aligned_img_list)))
                prewhitened_list =[]
                for i in range(len(aligned_img_list)):
                    logging.debug("type(aligned_img[{}])={}".format(i,type(aligned_img_list[i])))
                    prewhitened = facenet.prewhiten(aligned_img_list[i])
                    prewhitened_list.append(prewhitened)
                prewhitened_stack = np.stack(prewhitened_list)
                logging.debug("prewhitened_stack'shape={}".format(prewhitened_stack.shape))

                feed_dict = { "input:0": prewhitened_stack, "phase_train:0":True }
                logging.debug("begin run embeddings")
                emb = sess.run("embeddings:0", feed_dict=feed_dict)
                logging.debug("end run embedding, type(emb)={},emb.shape={}".format(type(emb),emb.shape))
                return emb
    load_model(model_path)



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

def view_graph_bydir(pathdir):
    with tf.Session(graph=tf.Graph()) as sess:
        tf.saved_model.loader.load(sess, ["serve"], pathdir)
        graph = tf.get_default_graph()
        [print(n.name) for n in tf.get_default_graph().as_graph_def().node]

def convertosimplemodel(pathdir,outsimplepath):
    with tf.Session(graph=tf.Graph()) as sess:
        tf.saved_model.loader.load(sess, ["serve"], pathdir)
        logging.info("begin save simple model")
        save_as_pb(sess,['detections'],outsimplepath)

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
    # view_effiect_bykaras()
    # view_effiect_tf1()
    # pbpath="/home/sailingzhang/winshare/develop/source/TONGWEI_SVN2/gentoo/trunk/products/cognition/facerecognition/embed_model/20180402-114759.pb"
    # pbpath="/home/sailingzhang/tmp/myraccoon_savemodel/saved_model.pb"
    pbpath = sys.argv[1]
    pbsimplepath= sys.argv[2]

    # view_graph_tf2(pbpath)
    convertosimplemodel(pbpath,pbsimplepath)
    logging.info("exit")
