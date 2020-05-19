from unittest import main, TestCase


from subprocess import Popen, PIPE

# https://stackoverflow.com/a/58696973
class TestEchoBasic(TestCase):

    def setUp(self):
        self.server = Popen(["./echoserver_noasan"])
        self.client = Popen(["./echoclient_noasan"], stdout=PIPE) 

    def tearDown(self):
        self.server.terminate()
        self.server.wait() #even after a child process exits, it remains a zombie until its parent calls wait
        self.client.terminate()
        self.client.wait()
    
    def testBasic(self):
        stdout, _ = self.client.communicate()
        print("stdout:", stdout.decode('utf-8'))

        assert "Hello World" in stdout.decode('utf-8')

class TestEchoMessage(TestCase):
    def setUp(self):
        self.server = Popen(["./echoserver_noasan"])
        self.client = Popen(["./echoclient_noasan", "-m", "Hello There World"], stdout=PIPE) 

    def tearDown(self):
        self.server.terminate()
        self.server.wait() #even after a child process exits, it remains a zombie until its parent calls wait
        self.client.terminate()
        self.client.wait()
    
    def testMessageInput(self):
        stdout, _ = self.client.communicate()
        print("stdout:", stdout.decode('utf-8'))

        assert "Hello There World" in stdout.decode('utf-8')

if __name__ == "__main__":
    main()