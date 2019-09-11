package app.main;

import app.consts.AppConsts;
import app.model.salt.SaltDataVO;
import app.utils.FileUtils;
import app.utils.XmlFileParser;

import java.io.File;
import java.util.List;

public class Main {

    public static void main(String[] args) throws Exception {
        System.out.println("Read Files");
        List<File> fileList = FileUtils.getInstance().getFileListInDir(AppConsts.sourceDirectoryPath);
        SaltDataVO saltDataVO = new SaltDataVO();

        for(File file : fileList){
            System.out.println("Parse File: " + file.getName());

            if(file.getName().contains("connection")){
                saltDataVO.setConnectionList(XmlFileParser.getInstance().parseConnectionFile(String.valueOf(file.toURI())));
            }else if(file.getName().contains("edge")){
                saltDataVO.setEdgeMap(XmlFileParser.getInstance().parseEdgeFile(String.valueOf(file.toURI())));
            }else if(file.getName().contains("node")){
                saltDataVO.setNodeMap(XmlFileParser.getInstance().parseNodeFile(String.valueOf(file.toURI())));
            }else if(file.getName().contains("tss")){
                saltDataVO.setTssMap(XmlFileParser.getInstance().parseTssFile(String.valueOf(file.toURI())));
            }else if(file.getName().contains("route")){
                //
            }else{
                System.out.println("Error in FileName: " + file.getName());
            }
        }

        System.out.println("Read File Complete");

        FileUtils.getInstance().writeLinkFileInDir(AppConsts.destDirectoryPath, "Link.dat", saltDataVO.getEdgeMap());
        //FileUtils.getInstance().writeVehicleFileInDir(AppConsts.destDirectoryPath, "Vehicle.dat", saltDataVO.getEdgeMap());

        //FileUtils.getInstance().writeFileInDir(AppConsts.destDirectoryPath, file.getName().replace("xml", "dat"), dataList);
        //System.out.println("Convert Complete");

    }

}
