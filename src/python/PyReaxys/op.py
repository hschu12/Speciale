import os
import shutil

from .env import *

class Op(object):
    def __init__(self, msg, msgFolder, srcFolder, tarFolder, op, begin=None, end=None, makeFolders=True):
        self.msg = msg
        self.msgFolder = msgFolder
        self.srcFolder = srcFolder
        self.tarFolder = tarFolder
        self.op = op
        self.begin = begin
        self.end = end
        self.makeFolders = makeFolders

    def __call__(self, fromSubfolder=1, toSubfolder=maxSubfolder):
        l.info(self.msg)
        l.info("\tsource = %s", self.srcFolder)
        l.info("\ttarget = %s", self.tarFolder)
        l.info("\tstart folder = %d", fromSubfolder)
        l.info("\tend folder   = %d", toSubfolder)
        temp = self.tarFolder + "_temp"
        if not self.makeFolders:
            if os.path.isdir(self.tarFolder):
                l.info("\trenaming existing folder to %s", temp)
                if os.path.isdir(temp):
                    l.info("\tremoving existing temp folder")
                    shutil.rmtree(temp)
                os.rename(self.tarFolder, temp)
            l.info("\tcreating target folder")
            os.mkdir(self.tarFolder)
        elif not os.path.isdir(self.tarFolder):
            l.info("\tcreating target folder")
            os.mkdir(self.tarFolder)
        if self.begin:
            self.begin(self)
        try:
            for i in range(fromSubfolder, toSubfolder + 1):
                l.info("\t%s" % (self.msgFolder % i))
                fSrc = self.srcFolder + "/%d" % i
                fTar = self.tarFolder + "/%d" % i
                if self.makeFolders:
                    temp = fTar + "_temp"
                    if os.path.isdir(fTar):
                        l.info("\t\trenaming existing folder to %s", temp)
                        if os.path.isdir(temp):
                            l.info("\t\tremoving existing temp folder")
                            shutil.rmtree(temp)
                        os.rename(fTar, temp)
                    l.info("\t\tcreating target folder")
                    os.mkdir(fTar)
                self.folderId = i
                self.op(self, fSrc, fTar)
                if self.makeFolders:
                    if os.path.isdir(temp):
                        l.info("\t\tremoving backup")
                        shutil.rmtree(temp)
            if os.path.isdir(temp) and not self.makeFolders:
                l.info("\tremoving backup")
                shutil.rmtree(temp)
        finally:
            if self.end:
                self.end(self)

