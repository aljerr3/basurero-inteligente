import React from 'react'
import chavez from '../img/chavez.png'

function Disclaimer() {
  return (
    <div className='content-disclaimer'>
        <h1 className='titleDisclaimer'>Charlemos con Chávez</h1>
        <img className='logoChavez' src={chavez} alt="" />
        <p className='content-disclaimer'>Ingresa cualquier pregunta en el formulario y la IA Chatvez te responderá. La pregunta puede ser cualquier cosa, preocupación personal o una pregunta mundana. Este modelo ha sido entrenado bajo el método fine-tuning, codificando los distintos discursos de Hugo Chávez con Whisper, y reentrenando los datasets.</p>
    </div>
  )
}

export default Disclaimer