package app.utils;

import app.model.edge.EdgeVO;

import java.io.*;
import java.net.URISyntaxException;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class FileUtils {

    private static FileUtils instance;

    public static FileUtils getInstance(){
        if(instance == null){
            instance = new FileUtils();
        }
        return instance;
    }

    public List<File> getFileListInDir(String folderPath) throws URISyntaxException {
        List<File> fileList = new ArrayList<>();

        File[] files = new File(getClass().getResource(folderPath).toURI()).listFiles();
        //If this pathname does not denote a directory, then listFiles() returns null.
        for (File file : files) {
            if (file.isFile()) {
                fileList.add(file);
            }
        }
        return fileList;
    }

    public boolean writeFileInDir(String targetFolderPath, String fileName, List dataList) throws IOException{
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(targetFolderPath + fileName), StandardCharsets.UTF_8));

        for(Object o : dataList){
            //System.out.println(o.toString());
            bw.write(o.toString());
            bw.newLine();
        }

        bw.close();

        return true;
    }

    public void writeLinkFileInDir(String targetFolderPath, String fileName, HashMap<String, EdgeVO> edgeMap) throws IOException {
        BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(targetFolderPath + fileName), StandardCharsets.UTF_8));
        StringBuilder sb = new StringBuilder();
        for(String edgeId : edgeMap.keySet()){
            EdgeVO edgeVO = edgeMap.get(edgeId);
            sb.append(edgeVO.getId()).append(" ").append(edgeVO.getNumLanes()).append(" ").append(edgeVO.getSpeed()).append(" ").append(edgeVO.getEdge_len());
            sb.append("\n");
        }
        bw.write(sb.toString());
        bw.close();
    }
}
