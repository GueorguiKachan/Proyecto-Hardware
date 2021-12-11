package es.unizar.eina.notepadv3;

import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class Test {
    private final NotesDbAdapter notes;
    public Test(NotesDbAdapter notes) {
        this.notes = notes;
    }

    public void pruebaVolumen(){
        String pre = new String();
        pre = "PRE-";
        for (int i = 0; i <1000; i++){
            notes.createNote(pre + i,"gueorgui");
        }
    }

    public void deshacerPruebaVolumen(){
        String pre = new String();
        pre = "PRE-";
        for (int i = 0; i <1000; i++){
            notes.deleteNoteByTitle(pre + i);
        }
    }

    public void pruebaSobreCarga(){
        String texto = "texto";
        int tamanyo = 5;
        while (true){
            Log.v("PruebaSobrecarga", "tamanyo="+ tamanyo);
            notes.createNote("sobrecarga - "+ tamanyo, texto);
            tamanyo += 5;
            texto += texto;
        }
    }
}
