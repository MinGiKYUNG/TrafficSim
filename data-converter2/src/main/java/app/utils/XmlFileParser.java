package app.utils;

import app.model.connection.ConnectionVO;
import app.model.edge.EdgeVO;
import app.model.node.NodeVO;
import app.model.tss.*;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class XmlFileParser {
    private static XmlFileParser instance;

    public static XmlFileParser getInstance (){
        if(instance == null){
            instance = new XmlFileParser();
        }
        return instance;
    }

    private Element readDocumentFromXML(String fileURI) throws ParserConfigurationException, IOException, SAXException {
        // XML 문서 파싱
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        DocumentBuilder documentBuilder = factory.newDocumentBuilder();
        Document document = documentBuilder.parse(fileURI);
        return document.getDocumentElement();
    }

    public List<ConnectionVO> parseConnectionFile(String fileURI) throws ParserConfigurationException, IOException, SAXException {
        List<ConnectionVO> connectionList = new ArrayList<>();
        ConnectionVO connectionVO;

        // root 구하기
        Element root = readDocumentFromXML(fileURI);

        NodeList children = root.getChildNodes(); // 자식 노드 목록 get
        for(int i = 0; i < children.getLength(); i++){
            Node node = children.item(i);
            if(node.getNodeType() == Node.ELEMENT_NODE){ // 해당 노드의 종류 판정(Element일 때)
                Element element = (Element)node;
                String nodeName = element.getNodeName();
                //System.out.println("node name: " + nodeName);

                if(nodeName.equals("connection")){
                    connectionVO = new ConnectionVO();

                    connectionVO.setFrom(element.getAttribute("from"));
                    connectionVO.setTo(element.getAttribute("to"));
                    connectionVO.setFromLane(element.getAttribute("fromLane"));
                    connectionVO.setToLane(element.getAttribute("toLane"));
                    if(element.hasAttribute("tl")){
                        connectionVO.setTl(element.getAttribute("tl"));
                    }else{
                        connectionVO.setTl(null);
                    }

                    if(element.hasAttribute("linkIndex")){
                        connectionVO.setLinkIndex(element.getAttribute("linkIndex"));
                    }else{
                        connectionVO.setLinkIndex(null);
                    }

                    connectionList.add(connectionVO);
                    //System.out.println(connectionVO.toString());
                }

            }
        }

        return connectionList;
    }

    public HashMap<String, EdgeVO> parseEdgeFile(String fileURI) throws ParserConfigurationException, IOException, SAXException {
        HashMap<String, EdgeVO> edgeMap = new HashMap<>();
        EdgeVO edgeVO;

        // root 구하기
        Element root = readDocumentFromXML(fileURI);

        NodeList children = root.getChildNodes(); // 자식 노드 목록 get
        for(int i = 0; i < children.getLength(); i++){
            Node node = children.item(i);
            if(node.getNodeType() == Node.ELEMENT_NODE){ // 해당 노드의 종류 판정(Element일 때)
                Element element = (Element)node;
                String nodeName = element.getNodeName();

                if(nodeName.equals("edge")){
                    edgeVO = new EdgeVO();

                    edgeVO.setId(element.getAttribute("id"));
                    edgeVO.setFrom(element.getAttribute("from"));
                    edgeVO.setTo(element.getAttribute("to"));
                    edgeVO.setPriority(element.getAttribute("priority"));
                    edgeVO.setNumLanes(element.getAttribute("numLanes"));
                    edgeVO.setSpeed(element.getAttribute("speed"));
                    edgeVO.setShape(element.getAttribute("shape"));
                    if(element.hasAttribute("spreadType")){
                        edgeVO.setSpreadType(element.getAttribute("spreadType"));
                    }else{
                        edgeVO.setSpreadType(null);
                    }

                    if(element.hasAttribute("edge_len")){
                        edgeVO.setEdge_len(element.getAttribute("edge_len"));
                    }else{
                        edgeVO.setEdge_len(null);
                    }

                    edgeMap.put(edgeVO.getId(), edgeVO);
                }

            }
        }

        return edgeMap;
    }

    public HashMap<String, NodeVO> parseNodeFile(String fileURI) throws ParserConfigurationException, IOException, SAXException {
        HashMap<String, NodeVO> nodeMap = new HashMap<>();
        NodeVO nodeVO;

        // root 구하기
        Element root = readDocumentFromXML(fileURI);

        NodeList children = root.getChildNodes(); // 자식 노드 목록 get
        for(int i = 0; i < children.getLength(); i++){
            Node node = children.item(i);
            if(node.getNodeType() == Node.ELEMENT_NODE){ // 해당 노드의 종류 판정(Element일 때)
                Element element = (Element)node;
                String nodeName = element.getNodeName();

                if(nodeName.equals("node")){
                    nodeVO = new NodeVO();

                    nodeVO.setId(element.getAttribute("id"));
                    nodeVO.setX(element.getAttribute("x"));
                    nodeVO.setY(element.getAttribute("y"));
                    nodeVO.setZ(element.getAttribute("z"));

                    if(element.hasAttribute("type")){
                        nodeVO.setType(element.getAttribute("type"));
                    }else{
                        nodeVO.setType(null);
                    }

                    if(element.hasAttribute("tl")){
                        nodeVO.setTl(element.getAttribute("tl"));
                    }else{
                        nodeVO.setTl(null);
                    }

                    nodeVO.setRadius(element.getAttribute("radius"));
                    nodeVO.setKeepClear(element.getAttribute("keepClear"));
                    nodeVO.setDong_cd(element.getAttribute("dong_cd"));

                    if(element.hasAttribute("partitionOwner")){
                        nodeVO.setPartitionOwner(element.getAttribute("partitionOwner"));
                    }else{
                        nodeVO.setPartitionOwner(null);
                    }

                    nodeMap.put(nodeVO.getId(), nodeVO);
                }
            }
        }
        return nodeMap;
    }


    public HashMap<String, TssVO> parseTssFile(String fileURI) throws ParserConfigurationException, IOException, SAXException {
        HashMap<String, TssVO> tssMap = new HashMap<>();

        // root 구하기
        Element root = readDocumentFromXML(fileURI);

        NodeList tssChildren = root.getChildNodes(); // 자식 노드 목록 get
        for(int i = 0; i < tssChildren.getLength(); i++){
            Node tssNode = tssChildren.item(i);
            if(tssNode.getNodeType() == Node.ELEMENT_NODE){ // 해당 노드의 종류 판정(Element일 때)
                Element tssElement = (Element)tssNode;
                if(tssElement.getNodeName().equals("trafficSignal")){
                    TssVO tssVO = new TssVO();

                    tssVO.setNodeID(tssElement.getAttribute("nodeID"));
                    tssVO.setVersion(tssElement.getAttribute("version"));
                    tssVO.setCrossNo(tssElement.getAttribute("crossNo"));
                    tssVO.setCrossName(tssElement.getAttribute("crossName"));
                    tssVO.setDate(tssElement.getAttribute("date"));
                    tssVO.setSignalGroup(tssElement.getAttribute("signalGroup"));

                    tssVO.setTodList(new ArrayList<>());
                    tssVO.setScheduleList(new ArrayList<>());

                    NodeList children = tssNode.getChildNodes();
                    for(int j = 0 ; j < children.getLength(); j++){
                        Node node = children.item(j);
                        if(node.getNodeType() == Node.ELEMENT_NODE) { // 해당 노드의 종류 판정(Element일 때)
                            Element element = (Element) node;
                            if(element.getNodeName().contains("tod")){
                                TodVO todVO = new TodVO();
                                todVO.setId(element.getAttribute("id"));
                                todVO.setTodSeq(element.getAttribute("todSeq"));
                                todVO.setScheduleID(element.getAttribute("scheduleID"));
                                todVO.setFromTime(element.getAttribute("fromTime"));
                                todVO.setToTime(element.getAttribute("toTime"));

                                tssVO.getTodList().add(todVO);
                            }else if(element.getNodeName().contains("schedule")){
                                ScheduleVO scheduleVO = new ScheduleVO();
                                scheduleVO.setOffset(element.getAttribute("offset"));
                                scheduleVO.setId(element.getAttribute("id"));

                                scheduleVO.setPhaseList(new ArrayList<>());

                                NodeList phaseChildren = node.getChildNodes();
                                for(int k = 0 ; k < phaseChildren.getLength(); k++){
                                    Node phaseNode = phaseChildren.item(k);
                                    if(phaseNode.getNodeType() == Node.ELEMENT_NODE) { // 해당 노드의 종류 판정(Element일 때)
                                        Element phaseElement = (Element) phaseNode;
                                        if (phaseElement.getNodeName().contains("phase")) {
                                            PhaseVO phaseVO = new PhaseVO();
                                            phaseVO.setState(phaseElement.getAttribute("state"));
                                            phaseVO.setDuration(phaseElement.getAttribute("duration"));

                                            scheduleVO.getPhaseList().add(phaseVO);
                                        }
                                    }
                                }
                                tssVO.getScheduleList().add(scheduleVO);
                            }else if(element.getNodeName().contains("TODPlan")){
                                TodPlanVO todPlanVO = new TodPlanVO();
                                todPlanVO.setOffset(element.getAttribute("offset"));
                                todPlanVO.setDefaultPlan(element.getAttribute("defaultPlan"));

                                todPlanVO.setPlanList(new ArrayList<>());

                                NodeList planChildren = node.getChildNodes();
                                for(int k = 0 ; k < planChildren.getLength(); k++){
                                    Node planNode = planChildren.item(k);
                                    if(planNode.getNodeType() == Node.ELEMENT_NODE) { // 해당 노드의 종류 판정(Element일 때)
                                        Element planElement = (Element) planNode;
                                        if (planElement.getNodeName().contains("plan")) {
                                            PlanVO planVO = new PlanVO();
                                            planVO.setOffset(planElement.getAttribute("offset"));
                                            planVO.setSchedule(planElement.getAttribute("schedule"));
                                            planVO.setStartTime(planElement.getAttribute("startTime"));

                                            todPlanVO.getPlanList().add(planVO);
                                        }
                                    }
                                }
                                tssVO.setTodPlan(todPlanVO);
                            }
                        }
                    }

                    System.out.println();
                    tssMap.put(tssVO.getNodeID(), tssVO);
                }
            }
        }
        return tssMap;
    }

}
